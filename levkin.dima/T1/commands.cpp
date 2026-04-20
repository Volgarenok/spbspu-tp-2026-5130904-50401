#include "commands.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "utils.hpp"

namespace levkin {

using Database = std::unordered_map< std::string, std::shared_ptr< Note > >;
using cmd_t = void (*)(std::istream&, std::ostream&, Database&);
using Cmds = std::unordered_map< std::string, cmd_t >;
Note::Note(std::string s) : id(std::move(s)), content(){};

void Note::addContent(std::string s) { content.push_back(std::move(s)); }

void Note::cleanExpired()
{
  eraseIf([](std::weak_ptr< Note >& curr) { return curr.expired(); });
}

void Note::rmLink(Link ptr)
{
  std::shared_ptr< Note > target = ptr.lock();
  if (!target)
    return;

  eraseIf([&target](const Link& curr) { return curr.lock() == target; });
}

void Note::addLink(Link ptr)
{
  for (Link curr : links) {
    if (curr.lock() == ptr.lock()) {
      throw std::logic_error("Double linking is not allowed");
    }
  }
  links.push_back(ptr);
}

const std::vector< Link >& Note::getLinks() const { return links; }

const std::vector< std::string >& Note::getContent() const { return content; }

void note(std::istream& in, std::ostream&, Database& db)
{
  std::string name = getWord(in);
  if (name.empty())
    return;

  if (db.find(name) != db.end()) {
    throw std::logic_error("already exists");
  }
  [name] = std::make_shared< Note >(name);
}

auto findNote(Database& db, std::string& name)
{
  auto it = db.find(name);
  if (it == db.end()) {
    throw std::logic_error("don't know this note yet");
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

const std::string& Note::getId() const { return id; }

void drop(std::istream& in, std::ostream&, Database& db)
{
  std::string name = getWord(in);
  if (db.erase(name) == 0) {
    throw std::logic_error("don't know this note yet");
  }
}

void link(std::istream& in, std::ostream&, Database& db)
{
  std::string from = getWord(in);
  std::string to = getWord(in);

  auto fromNoteIter = findNote(db, from);
  auto toNoteIter = findNote(db, to);

  fromNoteIter->second->addLink(toNoteIter->second);
}

void halt(std::istream& in, std::ostream&, Database& db)
{
  std::string from = getWord(in);
  std::string to = getWord(in);

  auto fromNoteIter = findNote(db, from);
  auto toNoteIter = findNote(db, to);

  fromNoteIter->second->rmLink(toNoteIter->second);
}

void mind(std::istream& in, std::ostream& out, Database& db)
{
  std::string name = getWord(in);
  auto it = findNote(db, name);

  std::shared_ptr< Note > note = it->second;

  for (auto link : note->getLinks()) {
    if (auto locked = link.lock()) {
      out << locked->getId();
      out << "\n";
    }
  }
}

void expired(std::istream& in, std::ostream& out, Database& db)
{
  std::string name = getWord(in);
  auto it = findNote(db, name);
  std::shared_ptr< Note > note = it->second;

  size_t count = 0;
  for (auto link : note->getLinks()) {
    if (link.expired()) {
      count++;
    }
  }
  out << count;
  out << "\n";
}

void refresh(std::istream& in, std::ostream&, Database& db)
{
  std::string name = getWord(in);
  auto it = findNote(db, name);
  std::shared_ptr< Note > note = it->second;

  note->cleanExpired();
}

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
