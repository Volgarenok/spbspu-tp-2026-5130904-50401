#ifndef COMMANDS
#define COMMANDS

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace levkin {
using Note = std::vector<std::string>;
using cmd_t = void (*)(std::istream&, std::ostream&, std::vector<Note>&);
using Cmds = std::unordered_map<std::string, cmd_t>;

void note(std::istream&, std::ostream&, std::vector<Note>&);

Cmds getCmds()
{
    Cmds cmds;

    cmds["note"] = note;

    return cmds;
}
}

#endif
