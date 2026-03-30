#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <limits>

void hello()
{
  std::cout << "Hello world!\n";
}


int main()
{
  using cmd_t = void(*)();
  std::unordered_map< std::string, cmd_t > cmds;
  cmds["hi!"] = hello;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)();
    }
    catch (const std::out_of_range&)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
    catch (const std::logic_error& e)
    {
      std::cout << "INVALID COMMAND: " << e.what() << ">\n";
    }
  }
}
