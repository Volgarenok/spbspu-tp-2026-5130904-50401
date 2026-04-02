#include <iostream>
#include <string>
#include <unordered_map>

int main()
{
  std::istream & input = std::cin;
  std::string cmd;
  while (input >> cmd) {

  }
  if (!input.eof()) {
    std::cerr << "bad input\n";
    return 1;
  }
}
