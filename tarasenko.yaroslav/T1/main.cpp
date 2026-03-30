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
  size_t expired_refs;
};

using unordered_map_of_notes = std::unordered_map< std::string, std::shared_ptr< Note > >;
using vector_of_strings = std::vector< std::string >;
using vector_of_weak_ptrs = std::vector< std::weak_ptr< Note > >;

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
  notes.emplace(name, std::make_shared< Note >(vector_of_strings{}, vector_of_weak_ptrs{}, 0));
}

void writeLine(std::istream& in, std::ostream&, unordered_map_of_notes& notes)
{
  std::string name;
  in >> name;
  if (notes.count(name) > 0)
  {
    throw std::logic_error ("No file with such name");
  }
  std::string line;
  in >> std::quoted(line);
  notes[name]->data.push_back(line);
}

int main()
{
  std::unordered_map< std::string, std::shared_ptr< Note > > notes;

  using cmd_t = void(*)(std::istream&, std::ostream&, unordered_map_of_notes&);
  std::unordered_map< std::string, cmd_t > cmds;
  cmds["hi!"] = hello;
  cmds["note"] = makeNote;

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
