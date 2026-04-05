#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <unordered_map>
#include <memory>

struct Note {
  std::string name;
  std::vector< std::string > lines;
  std::vector< std::weak_ptr< Note > > other_notes;

  Note(const std::string& n):
    name(n)
  {}
};

using NoteBook = std::unordered_map< std::string, std::shared_ptr< Note > >;

void addNote(std::istream& in, std::ostream&, NoteBook& notes)
{
  std::string name;
  in >> name;
  if (notes.find(name) != notes.end()) {
    throw std::logic_error("Note with this name already exist!");
  }
  notes[name] = std::make_shared< Note >(name);
}

void addLine(std::istream& in, std::ostream&, NoteBook& notes)
{
  std::string name, line;
  in >> name >> std::quoted(line);
  if (notes.find(name) == notes.end()) {
    throw std::logic_error("Note with this name doesnt exist!");
  }
  notes[name]->lines.push_back(line);
}

void showNote(std::istream& in, std::ostream& out, NoteBook& notes)
{
  std::string name;
  in >> name;
  if (notes.find(name) == notes.end()) {
    throw std::logic_error("Note with this name doesnt exist!");
  }
  auto note = notes[name];
  for (size_t i = 0; i < note->lines.size(); i++) {
    out << note->lines[i] << "\n";
  }
  if (note->lines.empty()) {
    out << "\n";
  }
}

void dropNote(std::istream&, std::ostream&, NoteBook& notes)
{

}

int main()
{
  NoteBook notes;
  using cmd_t = void (*)(std::istream&, std::ostream&, NoteBook&);
  std::unordered_map< std::string, cmd_t > cmds;
  cmds["note"] = addNote;
  cmds["line"] = addLine;
  cmds["show"] = showNote;
  cmds["drop"] = dropNote;
  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cin, std::cout, notes);
    } catch (const std::out_of_range&) {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    } catch (const std::logic_error& e) {
      std::cout << "<INVALID COMMAND: " << e.what() << ">\n";
    }
  }
  if (!std::cin.eof()) {
    std::cerr << "Bad input\n";
    return 1;
  }
}
