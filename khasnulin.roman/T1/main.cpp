#include <iostream>
#include <limits>
#include <string>

#include "cmds.hpp"

void skipLine(std::istream &in)
{
  auto toignore = std::numeric_limits< std::streamsize >::max();
  in.ignore(toignore, '\n');
}

int main()
{
  using namespace khasnulin;
  std::unordered_map< std::string, Cmd > cmds;

  cmds["note"] = addNote;
  cmds["line"] = addLine;
  cmds["show"] = showLine;
  cmds["drop"] = dropNote;
  cmds["link"] = linkNotes;
  cmds["mind"] = mindLinks;
  cmds["expired"] = expiredLinks;
  cmds["refresh"] = refreshLinks;
  cmds["halt"] = haltLink;

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
        skipLine(std::cin);
      }
    }
    catch (const std::exception &e)
    {
      std::cout << "<INVALID COMMAND>\n";
      skipLine(std::cin);
    }
  }
}
