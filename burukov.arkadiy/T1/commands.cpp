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
}
