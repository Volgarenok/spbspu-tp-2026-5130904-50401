#include <stdexcept>
#include <iomanip>

#include "commands.hpp"

void studilova::note(std::istream& in, std::ostream&, Context& ctx)
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

void studilova::line(std::istream& in, std::ostream&, Context& ctx)
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

  if (lines.empty())
  {
    out << "\n";
  }

  for (size_t i = 0; i < lines.size(); ++i)
  {
    out << lines[i] << "\n";
  }
}

void studilova::drop(std::istream& in, std::ostream&, Context& ctx)
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

void studilova::link(std::istream& in, std::ostream&, Context& ctx)
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

  for (size_t i = 0; i < links.size(); ++i)
  {
    auto s = links[i].lock();

    if (s && s->name == to)
    {
      throw std::logic_error("Duplicate");
    }
  }
  links.push_back(itTo->second);
}

void studilova::halt(std::istream& in, std::ostream&, Context& ctx)
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
  bool removed = false;

  for (auto it = links.begin(); it != links.end(); )
  {
    if (auto s = it->lock())
    {
      if (s->name == to)
      {
        it = links.erase(it);
        removed = true;
        continue;
      }
    }
    ++it;
  }

  if (!removed)
  {
    throw std::logic_error("Invalid");
  }
}

void studilova::mind(std::istream& in, std::ostream& out, Context& ctx)
{
  std::string name;
  in >> name;

  if (!in)
  {
    throw std::logic_error("Invalid");
  }

  auto it = ctx.notes.find(name);
  if(it == ctx.notes.end())
  {
    throw std::logic_error("Invalid");
  }

  const auto& links = it->second->links;
  bool printed = false;

  for (size_t i = 0; i < links.size(); ++i)
  {
    auto s = links[i].lock();

    if (s)
    {
      out << s->name << "\n";
      printed = true;
    }
  }

  if (!printed)
  {
    out << "\n";
  }
}

void studilova::expired(std::istream& in, std::ostream& out, Context& ctx)
{
  std::string name;
  in >> name;

  if(!in)
  {
    throw std::logic_error("Invalid");
  }

  auto it = ctx.notes.find(name);
  if (it == ctx.notes.end())
  {
    throw std::logic_error("Invalid");
  }

  auto& links = it->second->links;
  int count = 0;

  for(size_t i = 0; i < links.size(); ++i)
  {
    if (links[i].expired())
    {
      ++count;
    }
  }
  out << count << "\n";
}

void studilova::refresh(std::istream& in, std::ostream&, Context& ctx)
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

  auto& links = it->second->links;

  for (auto iter = links.begin(); iter != links.end(); )
  {
    if (iter->expired())
    {
      iter = links.erase(iter);
    } else {
      ++iter;
    }
  }
}
