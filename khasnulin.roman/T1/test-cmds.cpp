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
