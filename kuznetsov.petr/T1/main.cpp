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

void linkNotes(std::istream& in, std::ostream&, notepad_t& db)
{
  std::string from;
  std::string to;
  in >> from;
  in >> to;
  if (!db.count(from)) {
    throw std::logic_error("\"from note\" does not exist");
  }
  if (!db.count(to)) {
    throw std::logic_error("\"to note\" does not exist");
  }
  std::shared_ptr< kuz::Record > fromNote = db.at(from);
  std::shared_ptr< kuz::Record > toNote = db.at(to);
  fromNote->refs_.push_back(toNote);
}

void showLinks(std::istream& in, std::ostream& out, const notepad_t& db)
{
  std::string name;
  in >> name;
  if (!db.count(name)) {
    throw std::logic_error("this note does not exist");
  }
  const std::vector< std::weak_ptr< kuz::Record > >& links = db.at(name)->refs_;
  for (const auto link : links) {
    if (const std::shared_ptr< kuz::Record > spt = link.lock()) {
      out << spt->name_ << '\n';
    }
  }
}

void removeLink(std::istream& in, std::ostream&, notepad_t& db)
{
  std::string from;
  std::string to;
  in >> from;
  in >> to;
  if (!db.count(from)) {
    throw std::logic_error("\"from note\" does not exist");
  }
  if (!db.count(to)) {
    throw std::logic_error("\"to note\" does not exist");
  }
  std::shared_ptr< kuz::Record > fromNote = db.at(from);
  std::shared_ptr< kuz::Record > toNote = db.at(to);
  bool removed = false;
  auto iter = fromNote->refs_.begin();
  for (; iter != fromNote->refs_.end(); iter++) {
    if (std::shared_ptr< kuz::Record > curr = iter->lock()) {
      if (curr->name_ == toNote->name_) {
        fromNote->refs_.erase(iter);
        removed = true;
        break;
      }
    }
  }
  if (!removed) {
    throw std::logic_error("first note does not see second note");
  }
}

void getCountRemovedNotes(std::istream& in, std::ostream& out, const notepad_t& db)
{
  std::string name;
  in >> name;
  if (!db.count(name)) {
    throw std::logic_error("this note does not exist");
  }
  const std::shared_ptr< kuz::Record > from = db.at(name);
  const std::vector< std::weak_ptr< kuz::Record > >& links = from->refs_;
  size_t count = 0;
  auto iter = links.cbegin();
  for (; iter != links.cend(); iter++) {
    if (iter->expired()) {
      count++;
    }
  }
  out << count << '\n';
}

void removeRemovedLinks(std::istream& in, std::ostream& out, notepad_t& db)
{
  std::string name;
  in >> name;
  if (!db.count(name)) {
    throw std::logic_error("this note does not exist");
  }
  std::shared_ptr< kuz::Record > from = db.at(name);
  std::vector< std::weak_ptr< kuz::Record > >& links = from->refs_;
  auto iter = links.begin();
  while (iter != links.end()) {
    if (iter->expired()) {
      iter = links.erase(iter);
    } else {
      iter++;
    }
  }
}

int main()
{
  using command = void(*)(std::istream&, std::ostream&, notepad_t&);
  using constCommand = void(*)(std::istream&, std::ostream&, const notepad_t&);
  std::unordered_map< std::string, command> cmds;
  cmds["note"] = addNote;
  cmds["line"] = addLine;
  cmds["drop"] = drop;
  cmds["link"] = linkNotes;
  cmds["halt"] = removeLink;
  cmds["refresh"] = removeRemovedLinks;
  std::unordered_map< std::string, constCommand> constCmds;
  constCmds["show"] = show;
  constCmds["mind"] = showLinks;
  constCmds["expired"] = getCountRemovedNotes;
  notepad_t db;
  std::string cmd;
  while(std::cin >> cmd) {
    try {
      if (constCmds.count(cmd)) {
        constCmds.at(cmd)(std::cin, std::cout, db);
      } else if (cmds.count(cmd)) {
        cmds.at(cmd)(std::cin, std::cout, db);
      } else {
        std::cout << "<INVALID COMMAND>\n";
      }
    } catch (const std::logic_error& e) {
      std::cout << "<INVALID COMMAND: " << e.what() << ">\n";
    }
  }
  return 0;
}
