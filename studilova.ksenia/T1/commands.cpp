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
