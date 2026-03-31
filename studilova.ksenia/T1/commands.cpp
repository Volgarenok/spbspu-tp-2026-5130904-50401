#include <stdexcept>
#include <iomanip>

#include "commands.hpp"

void studilova::note(std::istream& in, std::ostream& out, Context& ctx)
{
  std::string name;
  in >> name;

  if (!in || ctx.notes.count(name))
  {
    throw std::logic_error("Invalid");
  }

  auto n = std::make_shared< Note >();
  n->name = name;

  ctx.notes[name] = n;
}

void studilova::line(std::istream& in, std::ostream& out, Context& ctx)
{
  std::string name;
  std::string text;

  in >> name >> std::quoted(text);

  if(!in)
  {
    throw std::logic_error("Invalid");
  }

  auto it = ctx.notes.find(name);
  if (it == ctx.notes.end())
  {
    throw std::logic_error("Invalid");
  }
  it->second->lines.push_back(text);
}

void studilova::show(std::istream& in, std::ostream& out, Context& ctx)
{
  std::string name;
  in >> name;

  if (!in)
  {
    throw std::logic_error("Invalid");
  }

  auto it = ctx.notes.find(name);
  if (it == ctx.notes.end())
  {
    throw std::logic_error("Invalid");
  }

  const auto& lines = it->second->lines;
  for (size_t i = 0; i < lines.size(); ++i)
  {
    out << lines[i] << "\n";
  }
}

void studilova::drop(std::istream& in, std::ostream& out, Context& ctx)
{
  std::string name;
  in >> name;

  if (!in)
  {
    throw std::logic_error("Invalid");
  }

  auto it = ctx.notes.find(name);
  if (it == ctx.notes.end())
  {
    throw std::logic_error("Invalid");
  }
  ctx.notes.erase(it);
}

void studilova::link(std::istream& in, std::ostream& out, Context& ctx)
{
  std::string from, to;
  in >> from >> to;

  if (!in)
  {
    throw std::logic_error("Invalid");
  }

  auto itFrom = ctx.notes.find(from);
  auto itTo = ctx.notes.find(to);

  if (itFrom == ctx.notes.end() || itTo == ctx.notes.end())
  {
    throw std::logic_error("Invalid");
  }

  auto& links = itFrom->second->links;

  for (const auto& w : links)
  {
    if (auto s = w.lock())
    {
      if (s->name == to)
      {
        throw std::logic_error("Duplicate");
      }
    }
  }
  links.push_back(itTo->second);
}