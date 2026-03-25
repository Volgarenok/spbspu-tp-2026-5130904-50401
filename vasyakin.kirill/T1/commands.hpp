#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <unordered_map>
#include <memory>
#include "note.hpp"

namespace vasyakin
{
  using NotesMap = std::unordered_map<std::string, std::shared_ptr<Note>>;

  void note(std::istream&, std::ostream&, NotesMap&);
  void line(std::istream&, std::ostream&, NotesMap&);
  void show(std::istream&, std::ostream&, NotesMap&);
  void drop(std::istream&, std::ostream&, NotesMap&);
  void link(std::istream&, std::ostream&, NotesMap&);
  void mind(std::istream&, std::ostream&, NotesMap&);
  void halt(std::istream&, std::ostream&, NotesMap&);
  void expired(std::istream&, std::ostream&, NotesMap&);
  void refresh(std::istream&, std::ostream&, NotesMap&);
}

#endif
