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

void note(std::istream& in, std::ostream& out, Database& db);
void line(std::istream& in, std::ostream& out, Database& db);
void show(std::istream& in, std::ostream& out, Database& db);
void drop(std::istream& in, std::ostream& out, Database& db);
void link(std::istream& in, std::ostream& out, Database& db);
void halt(std::istream& in, std::ostream& out, Database& db);
void mind(std::istream& in, std::ostream& out, Database& db);
void expired(std::istream& in, std::ostream& out, Database& db);
void refresh(std::istream& in, std::ostream& out, Database& db);

Cmds getCmds()
{
    Cmds cmds;

    cmds["note"] = note;

    return cmds;
}
}

#endif
