#include "commands.hpp"
#include <iomanip>

namespace vasyakin
{

  void vasyakin::note(std::istream& in, std::ostream&, NotesMap& notes)
  {
    std::string name;
    if (!(in >> name))
    {
      throw std::logic_error("");
    }

    if (notes.find(name) == notes.end())
    {
      notes[name] = std::make_shared< vasyakin::Note >(name);
    }
  }

  void vasyakin::line(std::istream& in, std::ostream&, NotesMap& notes)
  {
    std::string name, txt;
    if (!(in >> name >> std::quoted(txt)))
    {
      throw std::logic_error("");
    }

    auto it = notes.find(name);
    if (it == notes.end())
    {
      throw std::logic_error("");
    }
    it->second->noteLine(txt);
  }

  void vasyakin::show(std::istream& in, std::ostream& out, NotesMap& notes)
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
    it->second->noteShow();
  }
}