#include "notebook.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
int main()
{
  using namespace lukashevich;
  Notebook nb;
  using cmd_t = void (Notebook::*)(std::istream &, std::ostream &, const std::string &);
  std::unordered_map< std::string, cmd_t > cmds;
  cmds["note"] = &Notebook::note;
  cmds["line"] = &Notebook::line;
  cmds["show"] = &Notebook::show;
  cmds["drop"] = &Notebook::drop;
  cmds["link"] = &Notebook::link;
  cmds["halt"] = &Notebook::halt;
  cmds["mind"] = &Notebook::mind;
  cmds["expired"] = &Notebook::expired;
  cmds["refresh"] = &Notebook::refresh;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd);
      std::string name;
      std::cin >> name;
      (nb.*cmds.at(cmd))(std::cin, std::cout, name);
    }
    catch (const std::out_of_range &)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
    catch (const std::logic_error&)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  if (!std::cin.eof())
  {
    std::cerr << "Bad input\n";
    return 1;
  }
  return 0;
}
