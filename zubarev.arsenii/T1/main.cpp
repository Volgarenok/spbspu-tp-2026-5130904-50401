#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
void hi(std::ostream& out, std::istream&, std::vector< std::string >&)
{
  out << "Hello, username!\n";
}
void noop(std::ostream&, std::istream&, std::vector< std::string >&)
{}
void add_string(std::ostream&, std::istream& in, std::vector< std::string >& db)
{
  std::string str;
  in >> std::quoted(str);
  db.push_back(str);
}
void show_last(std::ostream& out, std::istream&, std::vector< std::string >& db)
{
  if (db.empty()) {
    throw std::logic_error("db is empty");
  }
  out << db.back() << '\n';
}
int main()
{
  using cmd_t=void (*)(std::ostream&, std::istream&, std::vector< std::string >&);
  std::vector< std::string > db;
  std::unordered_map< std::string,cmd_t > cmds;
  cmds["add"] = add_string;
  cmds["hello"] = hi;
  cmds["noop"] = noop;
  cmds["add"] = add_string;
  cmds["show-last"] = show_last;

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cout, std::cin, db);

    } catch (const std::out_of_range&) {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    } catch (const std::logic_error& e) {
      std::cout << "<INVALID COMMAND: " << e.what() << '\n';
    }
  }
  if (!std::cin.eof()) {
    std::cerr << "Bad input";
    return 1;
  }
}
