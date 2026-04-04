#include "commands.hpp"
#include <iomanip>

namespace sedov
{
  void note(std::istream & in, std::ostream &, NUM & notes)
  {
    std::string n;
    if (!(in >> n))
    {
      std::logic_error("");
    }
    if (notes.find(n) != notes.end())
    {
      throw std::logic_error("");
    }
    notes[n] = std::make_shared< sedov::Note >(n);
  }

  void line(std::istream & in, std::ostream &, NUM & notes)
  {
    std::string n, t;
    if (!(in >> n >> std::quoted(t)))
    {
      std::logic_error("");
    }
    auto it = notes.find(n);
    if (it == notes.end())
    {
      throw std::logic_error("");
    }
    it->second->nLine(t);
  }

  void show(std::istream & in, std::ostream &, NUM & notes)
  {
    std::string n;
    if (!(in >> n))
    {
      throw std::logic_error("");
    }
    auto it = notes.find(n);
    if (it == notes.end())
    {
      throw std::logic_error("");
    }
    it->second->nShow();
  }

  void drop(std::istream & in, std::ostream &, NUM & notes)
  {
    std::string n;
    if (!(in >> n))
    {
      throw std::logic_error("");
    }
    if (notes.erase(n) == 0)
    {
      std::logic_error("");
    }
  }

  void link(std::istream & in, std::ostream &, NUM & notes)
  {
    std::string nFrom, nTo;
    if (!(in >> nFrom >> nTo))
    {
      throw std::logic_error("");
    }
    auto fromIt = notes.find(nFrom);
    auto toIt = notes.find(nTo);
    if (fromIt == notes.end() || toIt == notes.end())
    {
      throw std::logic_error("");
    }
    fromIt->second->nLink(toIt->second);
  }

  void mind(std::istream & in, std::ostream &, NUM & notes)
  {
    std::string n;
    if (!(in >> n))
    {
      throw std::logic_error("");
    }
    auto it = notes.find(n);
    if (it == notes.end())
    {
      throw std::logic_error("");
    }
    it->second->nMind();
  }

  void halt(std::istream & in, std::ostream &, NUM & notes)
  {
    std::string nFrom, nTo;
    if (!(in >> nFrom >> nTo))
    {
      throw std::logic_error("");
    }
    auto fromIt = notes.find(nFrom);
    auto toIt = notes.find(nTo);
    if (fromIt == notes.end() || toIt == notes.end())
    {
      throw std::logic_error("");
    }
    fromIt->second->nHalt(toIt->second);
  }

  void expired(std::istream & in, std::ostream & out, NUM & notes)
  {
    std::string n;
    if (!(in >> n))
    {
      throw std::logic_error("");
    }
    auto it = notes.find(n);
    if (it == notes.end())
    {
      throw std::logic_error("");
    }
    out << it->second->nExpired() << "\n";
  }

  void refresh(std::istream & in, std::ostream &, NUM & notes)
  {
    std::string n;
    if (!(in >> n))
    {
      throw std::logic_error("");
    }
    auto it = notes.find(n);
    if (it == notes.end())
    {
      throw std::logic_error("");
    }
    it->second->nRefresh();
  }
}
