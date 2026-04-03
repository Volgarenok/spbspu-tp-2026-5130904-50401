#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <memory>
#include <unordered_map>

#include "note.hpp"

namespace burukov
{
  void note(std::istream& in, std::ostream& out, NotesMap& notes);
  void line(std::istream& in, std::ostream& out, NotesMap& notes);
  void show(std::istream& in, std::ostream& out, NotesMap& notes);
  void drop(std::istream& in, std::ostream& out, NotesMap& notes);
  void link(std::istream& in, std::ostream& out, NotesMap& notes);
  void mind(std::istream& in, std::ostream& out, NotesMap& notes);
  void halt(std::istream& in, std::ostream& out, NotesMap& notes);
  void expired(std::istream& in, std::ostream& out, NotesMap& notes);
  void refresh(std::istream& in, std::ostream& out, NotesMap& notes);
}

#endif
