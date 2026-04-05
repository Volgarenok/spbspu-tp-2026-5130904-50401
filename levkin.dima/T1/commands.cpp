#include "commands.hpp"
#include "utils.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
namespace levkin {
Note::Note(std::string s)
    : id(std::move(s))
    , content() {};

void Note::addContent(std::string s)
{
    content.push_back(std::move(s));
}
using Database = std::unordered_map<std::string, std::shared_ptr<Note>>;
using cmd_t = void (*)(std::istream&, std::ostream&, Database&);
using Cmds = std::unordered_map<std::string, cmd_t>;

void note(std::istream& in, std::ostream&, Database& db)
{
    std::string name = getWord(in);
    if (name.empty())
        return;
    db[name] = std::make_shared<Note>(name);
}

void line(std::istream& in, std::ostream&, Database& db)
{
    std::string name = getWord(in);
    auto it = db.find(name);

    if (it != db.end()) {
        it->second->addContent(getQuote(in));
    } else {
        throw std::logic_error("don't know this note yet\n");
    }
}
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
    cmds["line"] = line;
    cmds["show"] = show;
    cmds["drop"] = drop;
    cmds["link"] = link;
    cmds["halt"] = halt;
    cmds["mind"] = mind;
    cmds["expired"] = expired;
    cmds["refresh"] = refresh;
    return cmds;
}
}
