#include <iostream>
#include <limits>
#include "commands.hpp"

void handleError(std::ostream& out, std::istream& in)
{
  out << "<INVALID COMMAND>\n";
  auto toignore = std::numeric_limits< std::streamsize >::max();
  in.ignore(toignore, '\n');
}

int main()
{
  using namespace hvostov;
  NoteBook notes;
  using cmd_t = void (*)(std::istream&, std::ostream&, NoteBook&);
  std::unordered_map< std::string, cmd_t > cmds;
  cmds["note"] = addNote;
  cmds["line"] = addLine;
  cmds["show"] = showNote;
  cmds["drop"] = dropNote;
  std::string cmd;
  while (std::cin >> cmd) {
    try {
      if (cmds.find(cmd) != cmds.end()) {
        cmds.at(cmd)(std::cin, std::cout, notes);
      } else {
        handleError(std::cout, std::cin);
      }
    } catch (const std::logic_error& e) {
      handleError(std::cout, std::cin);
    }
  }
  if (!std::cin.eof()) {
    std::cerr << "Bad input\n";
    return 1;
  }
}
