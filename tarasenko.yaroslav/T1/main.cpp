#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <limits>
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>

struct Note
{
  std::string name;
  std::vector< std::string > data;
  std::vector< std::weak_ptr< Note > > refs;
  std::unordered_set< std::string > ref_names;
};

using unordered_map_of_notes = std::unordered_map< std::string, std::shared_ptr< Note > >;

void hello(std::istream&, std::ostream& out, unordered_map_of_notes&)
{
  out  << "Hello World!\n";
}

void makeNote(std::istream& in, std::ostream&, unordered_map_of_notes& notes)
{
  std::string name;
  in >> name;
  if (notes.count(name))
  {
    throw std::logic_error("Note with this name already exists");
  }
  auto shared = std::make_shared< Note >();
  shared->name = name;
  notes.emplace(name, shared);
}

void writeLine(std::istream& in, std::ostream&, unordered_map_of_notes& notes)
{
  std::string name;
  in >> name;
  if (notes.count(name) == 0)
  {
    throw std::logic_error ("No note with this name");
  }
  std::string line;
  in >> std::quoted(line);
  notes[name]->data.push_back(line);
}

void showNote(std::istream& in, std::ostream& out, unordered_map_of_notes& notes)
{
  std::string name;
  in >> name;
  if (notes.count(name) == 0)
  {
    throw std::logic_error ("No note with this name");
  }
  auto note = notes[name];
  for (size_t i = 0; i < note->data.size(); ++i)
  {
    out << note->data[i] << "\n";
  }
  if (note->data.empty())
  {
    out << "\n";
  }
}

void dropNote(std::istream& in, std::ostream&, unordered_map_of_notes& notes)
{
  std::string name;
  in >> name;
  if (notes.count(name) == 0)
  {
    throw std::logic_error ("No note with this name");
  }
  notes.erase(name);
}

void linkNote(std::istream& in, std::ostream&, unordered_map_of_notes& notes)
{
  std::string name_from;
  in >> name_from;
  if (notes.count(name_from) == 0)
  {
    throw std::logic_error ("No note-from with this name");
  }
  auto note = notes[name_from];
  std::string name_to;
  in >> name_to;
  if (notes.count(name_to) == 0)
  {
    throw std::logic_error ("No note-to with this name");
  }
  if (note->ref_names.count(name_to))
  {
    throw std::logic_error("Referense to this note already exists");
  }
  note->ref_names.insert(name_to);
  note->refs.push_back(std::weak_ptr< Note >(notes[name_to]));
}

void haltNote(std::istream& in, std::ostream&, unordered_map_of_notes& notes)
{
  std::string name_from;
  in >> name_from;
  if (notes.count(name_from) == 0)
  {
    throw std::logic_error ("No note-from with this name");
  }
  auto note = notes[name_from];
  std::string name_to;
  in >> name_to;
  if (notes.count(name_to) == 0)
  {
    throw std::logic_error("No referense to this note in " + name_from);
  }
  note->ref_names.erase(name_to);
  for (auto it = note->refs.begin(); it != note->refs.end(); ++it)
  {
    if (auto shared = it->lock())
    {
      if (shared->name == name_to)
      {
        note->refs.erase(it);
        break;
      }
    }
  }
}

void mindMap(std::istream& in, std::ostream& out, unordered_map_of_notes& notes)
{
  std::string name_from;
  in >> name_from;
  if (notes.count(name_from) == 0)
  {
    throw std::logic_error ("No note with this name");
  }
  auto note = notes[name_from];
  bool is_something_printed = false;
  for (size_t i = 0; i < note->refs.size(); ++i)
  {
    if (auto shared = note->refs[i].lock())
    {
      std::string name = shared->name;
      if (note->ref_names.count(name))
      {
        out << name << "\n";
        is_something_printed = true;
      }
    }
  }
  if (!is_something_printed)
  {
    out << "\n";
  }
}

void expiredLinks(std::istream& in, std::ostream& out, unordered_map_of_notes& notes)
{
  std::string name_from;
  in >> name_from;
  if (notes.count(name_from) == 0)
  {
    throw std::logic_error ("No note with this name");
  }
  auto note = notes[name_from];
  size_t count = 0;
  for (size_t i = 0; i < note->refs.size(); ++i)
  {
    if (note->refs[i].expired())
    {
      ++count;
    }
  }
  out << count << "\n";
}

void refreshLinks(std::istream& in, std::ostream&, unordered_map_of_notes& notes)
{
  std::string name_from;
  in >> name_from;
  if (notes.count(name_from) == 0)
  {
    throw std::logic_error ("No note with this name");
  }
  auto note = notes[name_from];
  std::vector< std::weak_ptr< Note > > new_refs;
  for (auto it = note->refs.begin(); it != note->refs.end();)
  {
    if (it->expired())
    {
      it = note->refs.erase(it);
    }
    else
    {
      ++it;
    }
  }
  note->ref_names.clear();
  for (size_t i = 0; i < note->refs.size(); ++i)
  {
    if (auto shared = note->refs[i].lock())
    {
      note->ref_names.insert(shared->name);
    }
  }
}

int main()
{
  std::unordered_map< std::string, std::shared_ptr< Note > > notes;

  using cmd_t = void(*)(std::istream&, std::ostream&, unordered_map_of_notes&);
  std::unordered_map< std::string, cmd_t > cmds;
  cmds["hi"] = hello;
  cmds["note"] = makeNote;
  cmds["line"] = writeLine;
  cmds["show"] = showNote;
  cmds["drop"] = dropNote;
  cmds["link"] = linkNote;
  cmds["halt"] = haltNote;
  cmds["mind"] = mindMap;
  cmds["expired"] = expiredLinks;
  cmds["refresh"] = refreshLinks;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, notes);
    }
    catch (const std::exception&)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.clear();
      std::cin.ignore(toignore, '\n');
    }
  }
}
