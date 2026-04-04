#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>

#include "commands.hpp"

int main()
{
  using namespace kondrat;

  db data;
  std::unordered_map< std::string, command > cmds;
  cmds["note"] = comNote;
  cmds["line"] = comLine;
  cmds["show"] = comShow;
  cmds["drop"] = comDrop;
  cmds["link"] = comLink;
  cmds["halt"] = comHalt;
  cmds["mind"] = comMind;
  cmds["expired"] = comExpired;
  cmds["refresh"] = comRefresh;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, data);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      if (std::cin.fail())
      {
        std::cin.clear();
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }

  return 0;
}