#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <limits>

int main()
{
  std::unordered_map< std::string, void(*)(std::vector< std::string >&) > data;
  data["note"] == ;
  data["line"] == ;
  data["show"] == ;
  data["drop"] == ;
  data["link"] == ;
  data["halt"] == ;
  data["mind"] == ;
  data["expired"] == ;
  data["refresh"] == ;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      /* code */
    }
    catch(...)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }

    if (!std::cin.eof())
    {
      std::cerr << "BAD INPUT!\n";
      return 1;
    }
  }
}
