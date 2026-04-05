#include<iostream>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <limits>
struct Note {
  std::string name;
  std::vector< std::string > text;
  std::vector< std::weak_ptr< Note >> pointers;
};

void create_node(std::istream & in,
  std::ostream & out,
  std::map<std::string, std::shared_ptr< Note >> & notebook)
{
  std::string name;
  if (!(in >> name)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (notebook.count(name) == 0) {
    auto n = std::make_shared< Note >();
    n->name = name;
    notebook.insert({name, n});
  }
}

int main()
{
  std::map<std::string, std::shared_ptr< Note >> notebook;
  using cmd_t = void(*)(std::istream & in,
    std::ostream & out,
    std::map<std::string, std::shared_ptr< Note >> & notebook);
  std::map< std::string, cmd_t > cmds;
  cmds["note"] = create_node;

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cin, std::cout, notebook);
    } catch (const std::out_of_range &) {
      std::cout << "<INVALID COMMAND>\n";
      auto toignor = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignor, '\n');
    }
  }
  if (std::cin.eof()) {
    return 0;
  } else {
    std::cerr << "bad input\n";
  }
}

