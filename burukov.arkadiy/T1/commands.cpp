#include "commands.hpp"

#include <iomanip>

namespace burukov
{
  void note(std::istream& in, std::ostream&, NotesMap& notes)
  {
    std::string name;

    if (!(in >> name))
    {
      throw std::logic_error("");
    }

    if (notes.find(name) != notes.end())
    {
      throw std::logic_error("");
    }

    notes[name] = std::make_shared< burukov::Note >(name);
  }

  void line(std::istream& in, std::ostream&, NotesMap& notes)
  {
    std::string name;
    std::string text;

    if (!(in >> name >> std::quoted(text)))
    {
      throw std::logic_error("");
    }

    auto it = notes.find(name);

    if (it == notes.end())
    {
      throw std::logic_error("");
    }

    it->second->addLine(text);
  }

  void show(std::istream& in, std::ostream&, NotesMap& notes)
  {
    std::string name;

    if (!(in >> name))
    {
      throw std::logic_error("");
    }

    auto it = notes.find(name);

    if (it == notes.end())
    {
      throw std::logic_error("");
    }

    it->second->showLines();
  }

  void drop(std::istream& in, std::ostream&, NotesMap& notes)
  {
    std::string name;

    if (!(in >> name))
    {
      throw std::logic_error("");
    }

    if (notes.erase(name) == 0)
    {
      throw std::logic_error("");
    }
  }

  void link(std::istream& in, std::ostream&, NotesMap& notes)
  {
    std::string fromName;
    std::string toName;

    if (!(in >> fromName >> toName))
    {
      throw std::logic_error("");
    }

    auto fromIt = notes.find(fromName);
    auto toIt = notes.find(toName);

    if (fromIt == notes.end() || toIt == notes.end())
    {
      throw std::logic_error("");
    }

    fromIt->second->addLink(toIt->second);
  }

  void mind(std::istream& in, std::ostream&, NotesMap& notes)
  {
    std::string name;

    if (!(in >> name))
    {
      throw std::logic_error("");
    }

    auto it = notes.find(name);

    if (it == notes.end())
    {
      throw std::logic_error("");
    }

    it->second->showLinks();
  }
}
