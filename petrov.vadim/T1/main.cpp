#include "notebook.hpp"
#include <limits>

int main()
{
  using namespace petrov;
  using cmd_t = void(Notebook::*)(std::ostream&, std::istream&, const std::string&);

  std::unordered_map< std::string, cmd_t > data;
  data["note"] = &Notebook::note;
  data["line"] = &Notebook::line;
  data["show"] = &Notebook::show;
  data["drop"] = &Notebook::drop;
  data["link"] = &Notebook::link;
  data["halt"] = &Notebook::halt;
  data["mind"] = &Notebook::mind;
  data["expired"] = &Notebook::expired;
  data["refresh"] = &Notebook::refresh;


  Notebook n;
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      data.at(cmd);
      std::string Arg;
      std::cin >> Arg;
      (n.*data.at(cmd))(std::cout, std::cin, Arg);
    }
    catch(...)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
  }

  if (!std::cin.eof())
  {
    std::cerr << "BAD INPUT!\n";
    return 1;
  }
}
