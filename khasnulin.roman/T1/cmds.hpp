#ifndef CMDS_HPP
#define CMDS_HPP

#include "Note.hpp"
#include <ostream>
#include <unordered_map>

namespace khasnulin
{
  using NoteMap = std::unordered_map< std::string, std::shared_ptr< khasnulin::Note > >;
  using Cmd = void (*)(std::istream &, std::ostream &, NoteMap &);

  void addNote(std::istream &in, std::ostream &out, NoteMap &notes);
  void addLine(std::istream &in, std::ostream &out, NoteMap &notes);
  void showLine(std::istream &in, std::ostream &out, NoteMap &notes);
  void dropNote(std::istream &in, std::ostream &out, NoteMap &notes);
  void linkNotes(std::istream &in, std::ostream &out, NoteMap &notes);
}

#endif
