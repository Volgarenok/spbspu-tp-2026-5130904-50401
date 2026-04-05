#include <iostream>
#include <unordered_map>

struct Note {
  std::string name_;
  std::vector< std::string > lines_;
  std::vector< std::weak_ptr< Note > > other_notes_;
};

void addNote(std::istream&, std::ostream&, std::unordered_map< Note >& notes)
{
  std::string name;
  in >> name;
  if (notes.find(name) != notes.end()) {
    throw std::logic_error("Note with this name already exist!");
  }
  notes[name] = std::make_shared< Note >{name};
}

void addLine(std::istream&, std::ostream&, std::unordered_map< Note >& notes)
{
}

void showNote(std::istream&, std::ostream&, std::unordered_map< Note >& notes)
{

}

void dropNote(std::istream&, std::ostream&, std::unordered_map< Note >& notes)
{

}

int main()
{
  std::unordered_map< std::string, Note > notes;
  using cmd_t = void (*)(std::istream&, std::ostream&, std::unordered_map< Note >&);
  std::unordered_map< std::string, cmd_t > cmds;
  cmds["note"] = note;
  cmds["line"] = line;
  cmds["show"] = show;
  cmds["drop"] = drop;
  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cin, std::cout, notes);
    } catch (const std::out_of_range&) {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streansize >::max();
      std::cin.ignore(toignore, '\n');
    } catch (const std::logic_error& e) {
      std::cout << "<INVALID COMMAND: " << e.what() << ">\n";
    }
  }
  if (!cin.eof()) {
    std::cerr << "Bad input\n";
    return 1;
  }
}
