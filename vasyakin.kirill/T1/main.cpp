#include <iostream>
#include <limits>
#include <unordered_map>
#include "commands.hpp"

int main()
{
  vasyakin::NotesMap notes;
  using cmd_t = void(*)(std::istream&, std::ostream&, vasyakin::NotesMap&);
  std::unordered_map< std::string, cmd_t > cmds;

  cmds["note"] = vasyakin::note;
  cmds["line"] = vasyakin::line;
  cmds["show"] = vasyakin::show;
  cmds["drop"] = vasyakin::drop;
  cmds["link"] = vasyakin::link;
  cmds["mind"] = vasyakin::mind;
  cmds["halt"] = vasyakin::halt;
  cmds["expired"] = vasyakin::expired;
  cmds["refresh"] = vasyakin::refresh;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, notes);
    }
    catch(...)
    {
      std::cout << "<INVALID COMMAND>" << '\n';
      auto toignore = std::numeric_limits<std::streamsize>::max();
      std::cin.ignore(toignore, '\n');
    }
  }
  return 0;
}
