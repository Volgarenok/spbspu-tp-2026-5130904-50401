#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "note.hpp"

namespace kondrat
{
  using db = std::unordered_map< std::string, std::shared_ptr< Note > >;
  using command = void(*)(std::istream &, std::ostream &, db &);

  void comNote(std::istream& in, std::ostream& out, db& data);
  void comLine(std::istream& in, std::ostream& out, db& data);
  void comShow(std::istream& in, std::ostream& out, db& data);
  void comDrop(std::istream& in, std::ostream& out, db& data);

  void comLink(std::istream& in, std::ostream& out, db& data);
  void comHalt(std::istream& in, std::ostream& out, db& data);
  void comMind(std::istream& in, std::ostream& out, db& data);
  void comExpired(std::istream& in, std::ostream& out, db& data);
  void comRefresh(std::istream& in, std::ostream& out, db& data);
}

#endif