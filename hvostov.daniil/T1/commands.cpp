#include "commands.hpp"
#include <iomanip>

void hvostov::addNote(std::istream& in, std::ostream&, NoteBook& notes)
{
  std::string name;
  in >> name;
  if (notes.find(name) != notes.end()) {
    throw std::logic_error("Note with this name already exist!");
  }
  notes[name] = std::make_shared< Note >(name);
}

void hvostov::addLine(std::istream& in, std::ostream&, NoteBook& notes)
{
  std::string name, line;
  in >> name >> std::quoted(line);
  if (notes.find(name) == notes.end()) {
    throw std::logic_error("Note with this name doesnt exist!");
  }
  notes[name]->lines.push_back(line);
}

void hvostov::showNote(std::istream& in, std::ostream& out, NoteBook& notes)
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

void hvostov::dropNote(std::istream& in, std::ostream&, NoteBook& notes)
{
  std::string name;
  in >> name;
  if (notes.find(name) == notes.end()) {
    throw std::logic_error("Note with this name doesnt exist");
  }
  notes.erase(name);
}
