#include <iostream>
#include <string>
#include "commands.hpp"

int main()
{
  std::string current;
  using namespace levkin;
  Cmds cmds = getCmds();
  Database db;

  while (std::cin >> current) {
    try {
      if (cmds.find(current) == cmds.end()) {
        throw std::logic_error("Unknown command");
      }
      cmds.at(current)(std::cin, std::cout, db);
    } catch (const std::exception& e) {
      std::cout << "<INVALID COMMAND>\n";
      std::string dummy;
      std::getline(std::cin, dummy);
    }
  }

  if (!std::cin.eof() && std::cin.fail()) {
    std::cerr << "Bad input\n";
    return 1;
  }
  return 0;
}
