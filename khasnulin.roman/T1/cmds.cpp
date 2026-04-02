#include "cmds.hpp"

#include <iomanip>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>

void khasnulin::addNote(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string name;
  in >> name;
  if (notes.find(name) == notes.end())
  {
    notes[name] = std::make_shared< khasnulin::Note >(name);
  }
  else
  {
    throw std::logic_error("note already exist!");
  }
}

void khasnulin::addLine(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string name;
  in >> name;
  std::string line;
  in >> std::quoted(line);
  if (notes.find(name) != notes.end())
  {
    notes[name]->lines.push_back(line);
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

void khasnulin::dropNote(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string name;
  in >> name;
  if (notes.find(name) != notes.end())
  {
    notes.erase(name);
  }
  else
  {
    throw std::logic_error("can't drop note. Note with such name doesn't exists");
  }
}

void khasnulin::linkNotes(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string noteFrom;
  std::string noteTo;
  in >> noteFrom >> noteTo;

  if (notes.find(noteTo) == notes.end() || notes.find(noteFrom) == notes.end())
  {
    throw std::logic_error("can't link notes: notes with such names doesn't exists");
  }

  if (notes[noteFrom]->links_names.count(noteTo) != 0)
  {
    throw std::logic_error("can't link notes: linking note already exists in links list");
  }

  notes[noteFrom]->links.push_back(notes[noteTo]);
  notes[noteFrom]->links_names.insert(noteTo);
}

void khasnulin::mindLinks(std::istream &in, std::ostream &out, NoteMap &notes)
{
  std::string name;
  in >> name;

  if (notes.find(name) == notes.end())
  {
    throw std::logic_error("can't mind links: note with such name doesn't exists");
  }

  for (const auto &note_ptr : notes[name]->links)
  {
    if (!note_ptr.expired())
    {
      out << note_ptr.lock()->name << "\n";
    }
  }
}

void khasnulin::expiredLinks(std::istream &in, std::ostream &out, NoteMap &notes)
{
  std::string name;
  in >> name;
  if (notes.find(name) == notes.end())
  {
    throw std::logic_error("can't count expired links: note with such name doesn't exists");
  }

  size_t count = 0;
  for (const auto &note_ptr : notes[name]->links)
  {
    if (note_ptr.expired())
    {
      ++count;
    }
  }
  out << count << "\n";
}

void khasnulin::refreshLinks(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string name;
  in >> name;
  if (notes.find(name) == notes.end())
  {
    throw std::logic_error("can't refresh links: note with such name doesn't exists");
  }

  auto it = notes[name]->links.begin();

  std::unordered_set< std::string > new_names;

  while (it != notes[name]->links.end())
  {
    std::weak_ptr< Note > &note_ptr = *it;
    if (note_ptr.expired())
    {
      it = notes[name]->links.erase(it);
    }
    else
    {
      new_names.insert(note_ptr.lock()->name);
      ++it;
    }
  }
  notes[name]->links_names = new_names;
}

void khasnulin::haltLink(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string noteFrom;
  std::string noteTo;
  in >> noteFrom >> noteTo;
  if (notes.find(noteFrom) == notes.end())
  {
    throw std::logic_error("can't halt link: note with such name doesn't exists");
  }
  if (notes[noteFrom]->links_names.count(noteTo) == 0)
  {
    throw std::logic_error("can't halt link: note link doesn't exists in current note");
  }

  auto it = notes[noteFrom]->links.begin();
  bool found = false;

  for (; it != notes[noteFrom]->links.end() && !found; ++it)
  {
    std::weak_ptr< Note > &note_ptr = *it;
    if (!note_ptr.expired() && note_ptr.lock()->name == noteTo)
    {
      notes[noteFrom]->links.erase(it);
      found = true;
    }
  }

  notes[noteFrom]->links_names.erase(noteTo);
}
