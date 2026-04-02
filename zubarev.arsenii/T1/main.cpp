#include "commands.hpp"
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

int main()
{
  using cmd_t = void (*)(std::istream&, std::ostream&, zubarev::NoteBook&);

  std::unordered_map< std::string, cmd_t > cmds;
  cmds["note"] = zubarev::cmd_note;
  cmds["line"] = zubarev::cmd_line;
  cmds["show"] = zubarev::cmd_show;
  cmds["drop"] = zubarev::cmd_drop;
  cmds["link"] = zubarev::cmd_link;
  cmds["halt"] = zubarev::cmd_halt;
  cmds["mind"] = zubarev::cmd_mind;
  cmds["expired"] = zubarev::cmd_expired;
  cmds["refresh"] = zubarev::cmd_refresh;

  std::string cmd;
  zubarev::NoteBook notebook;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cin, std::cout, notebook);
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
