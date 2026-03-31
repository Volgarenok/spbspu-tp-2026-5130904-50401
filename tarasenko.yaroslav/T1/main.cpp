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
  size_t expired_refs = 0;
};

using unordered_map_of_notes = std::unordered_map< std::string, std::shared_ptr< Note > >;

void hello(std::istream&, std::ostream& out, unordered_map_of_notes&)
{
  out  << "Hello, World!\n";
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
    throw std::logic_error ("No note-to with this name");
  }
  if (note->ref_names.count(name_to) == 0)
  {
    throw std::logic_error("No referense to this note in " + name_from);
  }
  note->ref_names.erase(name_to);
  ++note->expired_refs;
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
  for (size_t i = 0; i < note->refs.size(); ++i)
  {
    if (auto shared = note->refs[i].lock())
    {
      std::string name = shared->name;
      if (note->ref_names.count(name))
      {
        out << name << "\n";
      }
    }
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
  out << notes[name_from]->expired_refs << "\n";
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
  for (auto it = note->ref_names.begin(); it != note->ref_names.end();)
  {
    if (notes.count(*it) == 0)
    {
      it = note->ref_names.erase(it);
    }
    else
    {
      ++it;
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
    catch (const std::out_of_range&)
    {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    }
    catch (const std::logic_error& e)
    {
      std::cout << "<INVALID COMMAND: " << e.what() << ">\n";
    }
  }
}
