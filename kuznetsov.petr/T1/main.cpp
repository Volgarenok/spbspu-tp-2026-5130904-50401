#include <iostream>
#include "record.hpp"

namespace  kuz = kuznetsov;
using notepad_t = std::unordered_map< std::string, std::shared_ptr< kuz::Record > >;

void addNote(std::istream& in, notepad_t& db)
{
  std::string name;
  in >> name;
  if (db.count(name) != 0) {
    throw std::logic_error("This note already exists");
  }
  db[name] = std::make_shared< kuz::Record >(name);
}

int main()
{
  notepad_t db;
  std::string cmd;
  while(std::cin >> cmd) {
    if (cmd == "add") {
      addNote(std::cin, db);
    }

  }

  
  
}

