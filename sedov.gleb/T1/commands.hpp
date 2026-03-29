#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include <unordered_map>
#include <memory>
#include "note.hpp"

namespace sedov
{
  void note(std::istream &, std::ostream &, NUM &);
  void line(std::istream &, std::ostream &, NUM &);
  void show(std::istream &, std::ostream &, NUM &);
  void drop(std::istream &, std::ostream &, NUM &);
  void link(std::istream &, std::ostream &, NUM &);
  void mind(std::istream &, std::ostream &, NUM &);
  void halt(std::istream &, std::ostream &, NUM &);
  void expired(std::istream &, std::ostream &, NUM &);
  void refresh(std::istream &, std::ostream &, NUM &);
}

#endif
