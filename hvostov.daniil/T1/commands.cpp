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

void hvostov::linkNote(std::istream& in, std::ostream&, NoteBook& notes)
{
  std::string note_from, note_to;
  in >> note_from >> note_to;
  if (notes.find(note_from) == notes.end()) {
    throw std::logic_error("Note with this name doesnt exist");
  } else if (notes.find(note_to) == notes.end()) {
    throw std::logic_error("Note with this name doesnt exist");
  }
  notes[note_from]->linked_notes.push_back(notes[note_to]);
}

void hvostov::haltNote(std::istream& in, std::ostream&, NoteBook& notes)
{
  std::string note_from, note_to;
  in >> note_from >> note_to;
  if (notes.find(note_from) == notes.end()) {
    throw std::logic_error("Note with this name doesnt exist");
  } else if (notes.find(note_to) == notes.end()) {
    throw std::logic_error("Note with this name doesnt exist");
  }
  auto& links = notes[note_from]->linked_notes;
  for (auto it = links.begin(); it != links.end(); it++) {
    if (auto locked = it->lock()) {
      if (locked->name == note_to) {
        links.erase(it);
        return;
      }
    }
  }
  throw std::logic_error("Note doesnt have this link");
}

void hvostov::mindNote(std::istream& in, std::ostream& out, NoteBook& notes)
{
  std::string note_from;
  in >> note_from;
  if (notes.find(note_from) == notes.end()) {
    throw std::logic_error("Note with this name doesnt exist");
  }
  bool f = true;
  auto& links = notes[note_from]->linked_notes;
  for (auto it = links.begin(); it != links.end(); it++) {
    if (auto linked_note = it->lock()) {
      out << linked_note->name << "\n";
      f = false;
    }
  }
  if (f) {
    out << "\n";
  }
}

void hvostov::expiredNote(std::istream& in, std::ostream& out, NoteBook& notes)
{
  std::string note_from;
  in >> note_from;
  if (notes.find(note_from) == notes.end()) {
    throw std::logic_error("Note with this name doesnt exist");
  }
  size_t count = 0;
  auto& links = notes[note_from]->linked_notes;
  for (auto it = links.begin(); it != links.end(); it++) {
    if (it->expired()) {
      count++;
    }
  }
  out << count << "\n";
}

void hvostov::refreshNote(std::istream& in, std::ostream&, NoteBook& notes)
{
  std::string note_from;
  in >> note_from;
  if (notes.find(note_from) == notes.end()) {
    throw std::logic_error("Note with this name doesnt exist");
  }
  auto& links = notes[note_from]->linked_notes;
  for (auto it = links.begin(); it != links.end();) {
    if (it->expired()) {
      it = links.erase(it);
    } else {
      it++;
    }
  }
}
