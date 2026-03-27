#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iomanip>
#include <iosfwd>

#include "notebook.hpp"
namespace zubarev
{
  void cmd_note(std::istream& in, std::ostream&, NoteBook& notebook);
  void cmd_line(std::istream& in, std::ostream&, NoteBook& notebook);
  void cmd_show(std::istream& in, std::ostream& out, NoteBook& notebook); //
  void cmd_drop(std::istream& in, std::ostream&, NoteBook& notebook);

  void cmd_link(std::istream& in, std::ostream&, NoteBook& notebook);
  void cmd_halt(std::istream& in, std::ostream&, NoteBook& notebook);
  void cmd_mind(std::istream& in, std::ostream& out, NoteBook& notebook);    //
  void cmd_expired(std::istream& in, std::ostream& out, NoteBook& notebook); //
  void cmd_refresh(std::istream& in, std::ostream&, NoteBook& notebook);
}

#endif
