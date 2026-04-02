#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

#include "cmds.hpp"

BOOST_AUTO_TEST_SUITE(test_note_command)
using namespace khasnulin;

BOOST_AUTO_TEST_CASE(test_add_new_note)
{
  NoteMap notes;
  std::stringstream in("my_note");
  std::stringstream out;

  BOOST_CHECK(notes.empty());

  BOOST_CHECK_NO_THROW(addNote(in, out, notes));

  BOOST_CHECK_EQUAL(notes.size(), 1);
  BOOST_CHECK(notes.count("my_note") == 1);
  BOOST_CHECK_EQUAL(notes["my_note"]->name, "my_note");
}

BOOST_AUTO_TEST_CASE(test_add_duplicate_note_throws)
{
  NoteMap notes;
  notes["existing"] = std::make_shared< khasnulin::Note >("existing");

  std::stringstream in("existing");
  std::stringstream out;

  BOOST_CHECK_THROW(addNote(in, out, notes), std::logic_error);
}

BOOST_AUTO_TEST_CASE(test_recreate_after_drop)
{
  NoteMap notes;
  std::stringstream in1("temp");
  std::stringstream out1;

  addNote(in1, out1, notes);

  std::stringstream in2("temp");
  dropNote(in2, out1, notes);
  BOOST_CHECK_EQUAL(notes.size(), 0);

  std::stringstream in3("temp");
  BOOST_CHECK_NO_THROW(addNote(in3, out1, notes));
  BOOST_CHECK_EQUAL(notes.size(), 1);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_line_command)
using namespace khasnulin;

BOOST_AUTO_TEST_CASE(test_add_line_success)
{
  NoteMap notes;
  const std::string name = "work";
  notes[name] = std::make_shared< khasnulin::Note >(name);

  std::stringstream in("work \"Buy some milk and bread\"");
  std::stringstream out;

  BOOST_CHECK_NO_THROW(addLine(in, out, notes));

  BOOST_REQUIRE_EQUAL(notes[name]->lines.size(), 1);
  BOOST_CHECK_EQUAL(notes[name]->lines[0], "Buy some milk and bread");
}

BOOST_AUTO_TEST_CASE(test_add_multiple_lines)
{
  NoteMap notes;
  const std::string name = "todo";
  notes[name] = std::make_shared< khasnulin::Note >(name);

  std::stringstream in1("todo \"First line\"");
  std::stringstream in2("todo \"Second line\"");
  std::stringstream out;

  addLine(in1, out, notes);
  addLine(in2, out, notes);

  BOOST_REQUIRE_EQUAL(notes[name]->lines.size(), 2);
  BOOST_CHECK_EQUAL(notes[name]->lines[0], "First line");
  BOOST_CHECK_EQUAL(notes[name]->lines[1], "Second line");
}

BOOST_AUTO_TEST_CASE(test_add_line_to_non_existent_note_throws)
{
  NoteMap notes;

  std::stringstream in("ghost \"Hello?\"");
  std::stringstream out;

  BOOST_CHECK_THROW(addLine(in, out, notes), std::logic_error);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_show_command)
using namespace khasnulin;

BOOST_AUTO_TEST_CASE(test_show_output_format)
{
  NoteMap notes;
  const std::string name = "lectures";
  auto note = std::make_shared< khasnulin::Note >(name);
  note->lines.push_back("Math: 2+2=4");
  note->lines.push_back("Physics: F=ma");
  notes[name] = note;

  std::stringstream in("lectures");
  std::stringstream out;

  BOOST_CHECK_NO_THROW(showLine(in, out, notes));

  std::string expected = "Math: 2+2=4\nPhysics: F=ma\n";
  BOOST_CHECK_EQUAL(out.str(), expected);
}

BOOST_AUTO_TEST_CASE(test_show_empty_note_throws)
{
  NoteMap notes;
  notes["empty"] = std::make_shared< khasnulin::Note >("empty");

  std::stringstream in("empty");
  std::stringstream out;

  BOOST_CHECK_THROW(showLine(in, out, notes), std::logic_error);
}

BOOST_AUTO_TEST_SUITE_END()
