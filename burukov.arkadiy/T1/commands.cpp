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
}
