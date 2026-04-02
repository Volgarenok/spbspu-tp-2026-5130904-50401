#include <exception>

#include <iomanip>
#include <iostream>
#include <istream>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "Note.hpp"

using NoteMap = std::unordered_map< std::string, std::shared_ptr< khasnulin::Note > >;

void skipLine(std::istream &in)
{
  auto toignore = std::numeric_limits< std::streamsize >::max();
  in.ignore(toignore, '\n');
}

void addNote(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string name;
  in >> name;
  if (notes.find(name) == notes.end())
  {
    notes[name] = std::make_shared< khasnulin::Note >(name);
    std::cout << "succefully created " << name << "!\n";
  }
  else
  {
    throw std::logic_error("note already exist!");
  }
}

void addLine(std::istream &in, std::ostream &, NoteMap &notes)
{
  std::string name;
  in >> name;
  std::string line;
  in >> std::quoted(line);
  if (notes.find(name) != notes.end())
  {
    notes[name]->lines.push_back(line);
    std::cout << "successfully add line" << "\n";
  }
  else
  {
    throw std::logic_error("can't add line, note with such name doesn't exists");
  }
}

int main()
{
  using Cmd = void (*)(std::istream &, std::ostream &, NoteMap &);

  std::unordered_map< std::string, Cmd > cmds;

  cmds["note"] = addNote;
  cmds["line"] = addLine;

  NoteMap notes;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      if (cmds.find(cmd) != cmds.end())
      {
        cmds[cmd](std::cin, std::cout, notes);
      }

      else
      {
        std::cout << "<INVALID COMMAND>\n";
      }
    }
    catch (const std::exception &e)
    {
      std::cout << "<INVALID COMMAND: " << e.what() << ">\n";
    }
    skipLine(std::cin);
  }
}
