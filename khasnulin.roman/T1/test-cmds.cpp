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
  notes["existing"] = std::make_shared< Note >("existing");

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
  notes[name] = std::make_shared< Note >(name);

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
  notes[name] = std::make_shared< Note >(name);

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
  auto note = std::make_shared< Note >(name);
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
  notes["empty"] = std::make_shared< Note >("empty");

  std::stringstream in("empty");
  std::stringstream out;

  BOOST_CHECK_NO_THROW(showLine(in, out, notes));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_drop_command)
using namespace khasnulin;

BOOST_AUTO_TEST_CASE(test_drop_success)
{
  NoteMap notes;
  notes["to_delete"] = std::make_shared< Note >("to_delete");

  std::stringstream in("to_delete");
  std::stringstream out;

  BOOST_CHECK_EQUAL(notes.size(), 1);
  BOOST_CHECK_NO_THROW(dropNote(in, out, notes));

  BOOST_CHECK_EQUAL(notes.size(), 0);
  BOOST_CHECK(notes.find("to_delete") == notes.end());
}

BOOST_AUTO_TEST_CASE(test_drop_non_existent_throws)
{
  NoteMap notes;
  std::stringstream in("void");
  std::stringstream out;

  BOOST_CHECK_THROW(dropNote(in, out, notes), std::logic_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_link_command)
using namespace khasnulin;
BOOST_AUTO_TEST_CASE(test_link_success)
{
  NoteMap notes;
  notes["A"] = std::make_shared< Note >("A");
  notes["B"] = std::make_shared< Note >("B");

  std::stringstream in("A B");
  std::stringstream out;

  BOOST_CHECK_NO_THROW(linkNotes(in, out, notes));

  BOOST_REQUIRE_EQUAL(notes["A"]->links.size(), 1);
  BOOST_CHECK_EQUAL(notes["A"]->links_names.count("B"), 1);

  auto linked_note = notes["A"]->links[0].lock();
  BOOST_REQUIRE(linked_note != nullptr);
  BOOST_CHECK_EQUAL(linked_note->name, "B");
}
BOOST_AUTO_TEST_CASE(test_link_duplicate_throws)
{
  NoteMap notes;
  notes["A"] = std::make_shared< Note >("A");
  notes["B"] = std::make_shared< Note >("B");

  std::stringstream in1("A B");
  std::stringstream out;
  linkNotes(in1, out, notes);

  std::stringstream in2("A B");
  BOOST_CHECK_THROW(linkNotes(in2, out, notes), std::logic_error);
}

BOOST_AUTO_TEST_CASE(test_link_self_reference)
{
  NoteMap notes;
  notes["A"] = std::make_shared< Note >("A");

  std::stringstream in("A A");
  std::stringstream out;
  BOOST_CHECK_NO_THROW(linkNotes(in, out, notes));
  BOOST_CHECK_EQUAL(notes["A"]->links.size(), 1);
}

BOOST_AUTO_TEST_CASE(test_link_non_existent_notes_throws)
{
  NoteMap notes;
  notes["A"] = std::make_shared< Note >("A");

  std::stringstream out;

  std::stringstream in1("Ghost B");
  BOOST_CHECK_THROW(linkNotes(in1, out, notes), std::logic_error);

  std::stringstream in2("A Ghost");
  BOOST_CHECK_THROW(linkNotes(in2, out, notes), std::logic_error);
}

BOOST_AUTO_TEST_CASE(test_link_to_deleted_note)
{
  NoteMap notes;
  notes["A"] = std::make_shared< Note >("A");
  notes["B"] = std::make_shared< Note >("B");

  std::stringstream in1("A B");
  std::stringstream out;
  linkNotes(in1, out, notes);

  notes.erase("B");

  BOOST_REQUIRE_EQUAL(notes["A"]->links.size(), 1);
  BOOST_CHECK(notes["A"]->links[0].expired());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_link_command)

using namespace khasnulin;

BOOST_AUTO_TEST_CASE(test_mind_success_order)
{
  NoteMap notes;
  notes["A"] = std::make_shared< Note >("A");
  notes["B"] = std::make_shared< Note >("B");
  notes["C"] = std::make_shared< Note >("C");

  std::stringstream in_link1("A C");
  std::stringstream in_link2("A B");
  std::stringstream out;

  linkNotes(in_link1, out, notes);
  linkNotes(in_link2, out, notes);

  std::stringstream in_mind("A");
  std::stringstream out_mind;
  BOOST_CHECK_NO_THROW(mindLinks(in_mind, out_mind, notes));

  std::string expected = "C\nB\n";
  BOOST_CHECK_EQUAL(out_mind.str(), expected);
}

BOOST_AUTO_TEST_CASE(test_mind_skips_deleted_notes)
{
  NoteMap notes;
  notes["A"] = std::make_shared< Note >("A");
  notes["B"] = std::make_shared< Note >("B");
  notes["C"] = std::make_shared< Note >("C");

  std::stringstream out;
  std::stringstream in_l1("A B");
  linkNotes(in_l1, out, notes);
  std::stringstream in_l2("A C");
  linkNotes(in_l2, out, notes);

  notes.erase("B");

  std::stringstream in_mind("A");
  std::stringstream out_mind;
  mindLinks(in_mind, out_mind, notes);

  BOOST_CHECK_EQUAL(out_mind.str(), "C\n");
}

BOOST_AUTO_TEST_CASE(test_mind_empty_links)
{
  NoteMap notes;
  notes["A"] = std::make_shared< Note >("A");

  std::stringstream in_mind("A");
  std::stringstream out_mind;

  BOOST_CHECK_NO_THROW(mindLinks(in_mind, out_mind, notes));
  BOOST_CHECK_EQUAL(out_mind.str(), "");
}

BOOST_AUTO_TEST_CASE(test_mind_non_existent_source_throws)
{
  NoteMap notes;
  std::stringstream in_mind("Ghost");
  std::stringstream out_mind;

  BOOST_CHECK_THROW(mindLinks(in_mind, out_mind, notes), std::logic_error);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_expire_command)

using namespace khasnulin;

BOOST_AUTO_TEST_CASE(test_expired_count_zero_initially)
{
  NoteMap notes;
  notes["A"] = std::make_shared< Note >("A");
  notes["B"] = std::make_shared< Note >("B");

  std::stringstream in_link("A B");
  std::stringstream out;
  linkNotes(in_link, out, notes);

  std::stringstream in_exp("A");
  std::stringstream out_exp;
  BOOST_CHECK_NO_THROW(expiredLinks(in_exp, out_exp, notes));

  BOOST_CHECK_EQUAL(out_exp.str(), "0\n");
}

BOOST_AUTO_TEST_CASE(test_expired_count_after_drop)
{
  NoteMap notes;
  notes["A"] = std::make_shared< Note >("A");
  notes["B"] = std::make_shared< Note >("B");
  notes["C"] = std::make_shared< Note >("C");

  std::stringstream out;
  std::stringstream in_l1("A B");
  linkNotes(in_l1, out, notes);
  std::stringstream in_l2("A C");
  linkNotes(in_l2, out, notes);

  notes.erase("B");
  notes.erase("C");

  std::stringstream in_exp("A");
  std::stringstream out_exp;
  expiredLinks(in_exp, out_exp, notes);

  BOOST_CHECK_EQUAL(out_exp.str(), "2\n");
}

BOOST_AUTO_TEST_CASE(test_expired_partial_deletion)
{
  NoteMap notes;
  notes["A"] = std::make_shared< khasnulin::Note >("A");
  notes["B"] = std::make_shared< khasnulin::Note >("B");
  notes["C"] = std::make_shared< khasnulin::Note >("C");

  std::stringstream out;
  std::stringstream in_l1("A B");
  linkNotes(in_l1, out, notes);
  std::stringstream in_l2("A C");
  linkNotes(in_l2, out, notes);

  notes.erase("B");

  std::stringstream in_exp("A");
  std::stringstream out_exp;
  expiredLinks(in_exp, out_exp, notes);

  BOOST_CHECK_EQUAL(out_exp.str(), "1\n");
}

BOOST_AUTO_TEST_CASE(test_expired_non_existent_source_throws)
{
  NoteMap notes;
  std::stringstream in_exp("Ghost");
  std::stringstream out_exp;

  BOOST_CHECK_THROW(expiredLinks(in_exp, out_exp, notes), std::logic_error);
}

BOOST_AUTO_TEST_SUITE_END()
