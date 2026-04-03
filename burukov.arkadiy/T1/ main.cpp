#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>

#include "commands.hpp"

int main()
{
  burukov::NotesMap notes;

  using cmd_t = void (*)(std::istream&, std::ostream&, burukov::NotesMap&);
  std::unordered_map< std::string, cmd_t > cmds;

  cmds["note"] = burukov::note;
  cmds["line"] = burukov::line;
  cmds["show"] = burukov::show;
  cmds["drop"] = burukov::drop;
  cmds["link"] = burukov::link;
  cmds["mind"] = burukov::mind;
  cmds["halt"] = burukov::halt;
  cmds["expired"] = burukov::expired;
  cmds["refresh"] = burukov::refresh;

  std::string cmd;

  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, notes);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>" << '\n';
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  return 0;
}
