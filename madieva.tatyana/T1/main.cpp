#include<iostream>
#include <map>
#include <string>
#include <vector>
#include <memory> 
struct Note {
  std::string name;
  std::vector< std::string > text;
  std::vector< std::weak_ptr< Note >> pointers;
};

int main()
{
  std::map<std::string, std::shared_ptr< Note >> notebook;
  using cmd_t = void(*)();
  std::map< std::string, cmd_t > cmds;
  std::string line;

  while (std::cin >> line) {

  }
  if (std::cin.eof()) {
    return 0;
  } else {
    std::cerr << "bad input\n";
  }
}
