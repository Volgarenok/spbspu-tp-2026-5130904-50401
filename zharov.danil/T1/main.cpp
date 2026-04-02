#include <iostream>
#include <limits>
#include "notebook.hpp"


int main()
{
  zharov::Book notebook;
  std::string cmd;
  while (std::cin >> cmd) {
    try {
      auto cmd_func = notebook.cmds_.at(cmd);
      (notebook.*cmd_func)(std::cin, std::cout);
    } catch (const std::out_of_range&) {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    } catch (const std::logic_error& e) {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  if (!std::cin.eof()) {
    std::cerr << "Bad input\n";
    return 1;
  }
}
