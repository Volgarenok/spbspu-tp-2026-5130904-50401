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

void Note::rmLink(Link ptr)
{
    std::shared_ptr<Note> target = ptr.lock();
    if (!target)
        return;

    auto destination = links.begin();
    size_t writeIndex = 0;
    for (size_t readIndex = 0; readIndex < links.size(); ++readIndex) {
        if (links[readIndex].lock() != target) {
            links[writeIndex] = links[readIndex];
            writeIndex++;
        }
    }
    links.resize(writeIndex);
}

void Note::addLink(Link ptr)
{
    for (Link curr : links) {
        if (curr.lock() == ptr.lock()) {
            throw std::logic_error("Double linking is not allowed\n");
        }
    }
    links.push_back(ptr);
}

const std::vector<Link>& Note::getLinks() const
{
    return links;
}

const std::vector<std::string>& Note::getContent() const
{
    return content;
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

auto findNote(Database& db, std::string& name)
{
    auto it = db.find(name);
    if (it == db.end()) {
        throw std::logic_error("don't know this note yet\n");
    }
    return it;
}

void line(std::istream& in, std::ostream&, Database& db)
{
    std::string name = getWord(in);
    auto it = findNote(db, name);

    it->second->addContent(getQuote(in));
}

void show(std::istream& in, std::ostream& out, Database& db)
{
    std::string name = getWord(in);
    auto it = findNote(db, name);

    for (std::string str : it->second->getContent()) {
        out << str;
        out << "\n";
    }
}

void drop(std::istream& in, std::ostream& out, Database& db)
{
    std::string name = getWord(in);
    if (db.erase(name) == 0) {
        throw std::logic_error("don't know this note yet\n");
    }
}

void link(std::istream& in, std::ostream& out, Database& db)
{
}
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
