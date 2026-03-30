#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <limits>
#include <string>
#include <vector>
#include <memory>

struct Note
{
  std::vector< std::string > data;
  std::vector< std::weak_ptr< Note > > refs;
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
  if (notes.count(name) > 0)
  {
    throw std::logic_error("Note with this name already exists");
  }
  notes.emplace(name, std::make_shared< Note >());
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
