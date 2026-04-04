#include <iostream>
#include <limits>
#include "commands.hpp"

int main()
{
  sedov::NUM notes;
  using cmd_t = void(*)(std::istream &, std::ostream &, sedov::NUM &);
  std::unordered_map< std::string, cmd_t > cmds;

  cmds["note"] = sedov::note;
  cmds["line"] = sedov::line;
  cmds["show"] = sedov::show;
  cmds["drop"] = sedov::drop;
  cmds["link"] = sedov::link;
  cmds["mind"] = sedov::mind;
  cmds["halt"] = sedov::halt;
  cmds["expired"] = sedov::expired;
  cmds["refresh"] = sedov::refresh;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, notes);
    }
    catch(...)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits<std::streamsize>::max();
      std::cin.ignore(toignore, '\n');
    }
  }
  return 0;
}
