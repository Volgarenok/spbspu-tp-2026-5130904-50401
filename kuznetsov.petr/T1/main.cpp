#include <iostream>
#include <iomanip>
#include "record.hpp"

namespace  kuz = kuznetsov;
using notepad_t = std::unordered_map< std::string, std::shared_ptr< kuz::Record > >;

void addNote(std::istream& in, std::ostream& , notepad_t& db)
{
  std::string name;
  in >> name;
  if (db.count(name) != 0) {
    throw std::logic_error("This note already exists");
  }
  db[name] = std::make_shared< kuz::Record >(name);
}

void addLine(std::istream& in, std::ostream&, notepad_t& db)
{
  std::string name;
  in >> name;
  try {
    std::vector< std::string >& lines = db.at(name)->lines_;
    std::string line;
    in >> std::quoted(line);
    lines.push_back(line);
  } catch (...) {
    throw std::logic_error("this note does not exist");
  }
}

void show(std::istream& in, std::ostream& out, const notepad_t& db)
{
  std::string name;
  in >> name;
  try {
    const std::vector< std::string >& lines = db.at(name)->lines_;
    for (const std::string& str : lines) {
      out << str << '\n';
    }
  } catch (...) {
    throw std::logic_error("this note does not exist");
  }
}

void drop(std::istream& in, std::ostream&, notepad_t& db)
{
  std::string name;
  in >> name;
  if (db.count(name) == 0) {
    throw std::logic_error("such note does not exist");
  }
  db.erase(name);
}

int main()
{
  notepad_t db;
  std::string cmd;
  while(std::cin >> cmd) {
    if (cmd == "note") {
      addNote(std::cin, std::cout, db);
    } else if (cmd == "show") {
      show(std::cin, std::cout, db);
    } else if (cmd == "line") {
      addLine(std::cin, std::cout, db);
    } else if (cmd == "drop") {
      drop(std::cin, std::cout, db);
    }

  }
  
}

