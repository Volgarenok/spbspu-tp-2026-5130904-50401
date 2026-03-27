#include <iostream>
#include <unordered_map>
#include <limits>

#include "commands.hpp"

int main()
{
  studilova::Context ctx;
  std::unordered_map< std::string, studilova::cmd_t > cmds;

  cmds["note"] = studilova::note;
  cmds["line"] = studilova::line;
  cmds["show"] = studilova::show;
  cmds["drop"] = studilova::drop;

  cmds["link"] = studilova::link;
  cmds["halt"] = studilova::halt;
  cmds["mind"] = studilova::mind;
  cmds["expired"] = studilova::expired;
  cmds["refresh"] = studilova::refresh;

  std::string cmd;

  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, ctx);
    }
    catch(const std::out_of_range&)
    {
      std::cout << "<INVALID COMMAND>\n";

      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
