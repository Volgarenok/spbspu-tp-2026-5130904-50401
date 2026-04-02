#include "cmds.hpp"

#include <iomanip>
#include <ostream>
#include <stdexcept>

void khasnulin::addNote(std::istream &in, std::ostream &out, NoteMap &notes)
{
  std::string name;
  in >> name;
  if (notes.find(name) == notes.end())
  {
    notes[name] = std::make_shared< khasnulin::Note >(name);
    out << "succefully created " << name << "!\n";
  }
  else
  {
    throw std::logic_error("note already exist!");
  }
}

void khasnulin::addLine(std::istream &in, std::ostream &out, NoteMap &notes)
{
  std::string name;
  in >> name;
  std::string line;
  in >> std::quoted(line);
  if (notes.find(name) != notes.end())
  {
    notes[name]->lines.push_back(line);
    out << "successfully add line" << "\n";
  }
  else
  {
    throw std::logic_error("can't add line, note with such name doesn't exists");
  }
}

void khasnulin::showLine(std::istream &in, std::ostream &out, NoteMap &notes)
{
  std::string name;
  in >> name;
  if (notes.find(name) != notes.end())
  {
    if (notes[name]->lines.empty())
    {
      throw std::logic_error("can't show lines. Note doesn't exists any lines");
    }
    for (const auto &it : notes[name]->lines)
    {
      out << it << "\n";
    }
  }
  else
  {
    throw std::logic_error("can't show lines of such note. Note doesn't exists");
  }
}

void khasnulin::dropNote(std::istream &in, std::ostream &out, NoteMap &notes)
{
  std::string name;
  in >> name;
  if (notes.find(name) != notes.end())
  {
    notes.erase(name);
    out << "succefully droped note " << name << "\n";
  }
  else
  {
    throw std::logic_error("can't drop note. Note with such name doesn't exists");
  }
}

void khasnulin::linkNotes(std::istream &in, std::ostream &out, NoteMap &notes)
{
  std::string name;
  in >> name;
  out << name;
  notes.begin();
}
