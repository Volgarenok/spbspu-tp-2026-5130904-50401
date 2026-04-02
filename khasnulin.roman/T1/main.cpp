#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>

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

  std::unordered_set< std::string > outputCmds{"show", "mind", "expired"};

  NoteMap notes;

  std::string cmd;

  bool outputDone = false;
  while (std::cin >> cmd)
  {
    try
    {
      if (cmds.find(cmd) != cmds.end())
      {
        cmds[cmd](std::cin, std::cout, notes);
        if (outputCmds.find(cmd) != outputCmds.end())
        {
          outputDone = true;
        }
      }
      else
      {
        std::cout << "<INVALID COMMAND>\n";
        outputDone = true;
        skipLine(std::cin);
      }
    }
    catch (const std::exception &e)
    {
      std::cout << "<INVALID COMMAND>\n";
      outputDone = true;
      skipLine(std::cin);
    }
  }
  if (!outputDone)
  {
    std::cout << "\n";
  }
}
