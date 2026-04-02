#include "commands.hpp"
#include <iomanip>

namespace vasyakin
{

  void note(std::istream& in, std::ostream&, NotesMap& notes)
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

  void line(std::istream& in, std::ostream&, NotesMap& notes)
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
    it->second->noteShow();
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
    std::string from_name, to_name;
    if (!(in >> from_name >> to_name))
    {
      throw std::logic_error("");
    }

    auto from_it = notes.find(from_name);
    auto to_it = notes.find(to_name);
    if (from_it == notes.end() || to_it == notes.end())
    {
      throw std::logic_error("");
    }
    from_it->second->noteLink(to_it->second);
  }

  void halt(std::istream& in, std::ostream&, NotesMap& notes)
  {
    std::string from_name, to_name;
    if (!(in >> from_name >> to_name))
    {
      throw std::logic_error("");
    }

    auto from_it = notes.find(from_name);
    auto to_it = notes.find(to_name);
    if (from_it == notes.end() || to_it == notes.end())
    {
      throw std::logic_error("");
    }
    from_it->second->noteHalt(to_it->second);
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
    it->second->noteMind();
  }

  void expired(std::istream& in, std::ostream& out, NotesMap& notes)
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
    out << it->second->noteExpired() << '\n';
  }

  void refresh(std::istream& in, std::ostream&, NotesMap& notes)
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
    it->second->noteRefresh();
  }
}
