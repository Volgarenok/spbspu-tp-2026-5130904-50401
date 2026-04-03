#include <iostream>
#include <iomanip>
#include <limits>
#include "record.hpp"
#include "commands.hpp"

int main()
{
  namespace kuz = kuznetsov;
  using command = void(*)(std::istream&, std::ostream&, kuz::notepad_t&);
  using constCommand = void(*)(std::istream&, std::ostream&, const kuz::notepad_t&);
  std::unordered_map< std::string, command> cmds;
  cmds["note"] = kuz::addNote;
  cmds["line"] = kuz::addLine;
  cmds["drop"] = kuz::drop;
  cmds["link"] = kuz::linkNotes;
  cmds["halt"] = kuz::removeLink;
  cmds["refresh"] = kuz::removeRemovedLinks;
  std::unordered_map< std::string, constCommand> constCmds;
  constCmds["show"] = kuz::show;
  constCmds["mind"] = kuz::showLinks;
  constCmds["expired"] = kuz::getCountRemovedNotes;
  kuz::notepad_t db;
  std::string cmd;
  while(std::cin >> cmd) {
    try {
      if (constCmds.count(cmd)) {
        constCmds.at(cmd)(std::cin, std::cout, db);
      } else {
        cmds.at(cmd)(std::cin, std::cout, db);
      }
    } catch (const std::logic_error& e) {
      std::cout << "<INVALID COMMAND>\n";
      auto skip = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(skip, '\n');
    }
  }
  return 0;
}
