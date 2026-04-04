#include "commands.hpp"

#include <iomanip>
#include <stdexcept>

namespace kondrat
{
  std::shared_ptr< Note > findNote(const std::string & name, db & data)
  {
    if (data.find(name) == data.end())
    {
      throw std::logic_error("err");
    }
    return data[name];
  }

  bool hasLink(const std::shared_ptr< Note > & from, const std::shared_ptr< Note > & to)
  {
    for (size_t i = 0; i < from->links_.size(); ++i)
    {
      std::shared_ptr< Note > ptr = from->links_[i].lock();
      if (ptr && ptr == to)
      {
        return true;
      }
    }
    return false;
  }

  void comNote(std::istream & in, std::ostream &, db & data)
  {
    std::string name;
    if (!(in >> name))
    {
      throw std::logic_error("err");
    }

    if (data.find(name) != data.end())
    {
      throw std::logic_error("err");
    }

    std::shared_ptr< Note > note = std::make_shared< Note >();
    note->name_ = name;
    data[name] = note;
  }

  void comLine(std::istream & in, std::ostream &, db & data)
  {
    std::string name;
    std::string text;
    if (!(in >> name >> std::quoted(text)))
    {
      throw std::logic_error("err");
    }

    std::shared_ptr< Note > note = findNote(name, data);
    note->lines_.push_back(text);
  }

  void comShow(std::istream & in, std::ostream & out, db & data)
  {
    std::string name;
    if (!(in >> name))
    {
      throw std::logic_error("err");
    }

    std::shared_ptr< Note > note = findNote(name, data);

    for (size_t i = 0; i < note->lines_.size(); ++i)
    {
      out << note->lines_[i] << '\n';
    }
  }

  void comDrop(std::istream & in, std::ostream &, db & data)
  {
    std::string name;
    if (!(in >> name))
    {
      throw std::logic_error("err");
    }

    if (data.erase(name) == 0)
    {
      throw std::logic_error("err");
    }
  }

  void comLink(std::istream & in, std::ostream &, db & data)
  {
    std::string from;
    std::string to;
    if (!(in >> from >> to))
    {
      throw std::logic_error("err");
    }

    std::shared_ptr< Note > fromNote = findNote(from, data);
    std::shared_ptr< Note > toNote = findNote(to, data);

    if (hasLink(fromNote, toNote))
    {
      throw std::logic_error("err");
    }

    fromNote->links_.push_back(toNote);
  }

  void comHalt(std::istream & in, std::ostream &, db & data)
  {
    std::string from;
    std::string to;
    if (!(in >> from >> to))
    {
      throw std::logic_error("err");
    }

    std::shared_ptr< Note > fromNote = findNote(from, data);
    std::shared_ptr< Note > toNote = findNote(to, data);

    for (size_t i = 0; i < fromNote->links_.size(); ++i)
    {
      std::shared_ptr< Note > ptr = fromNote->links_[i].lock();
      if (ptr && ptr == toNote)
      {
        fromNote->links_.erase(fromNote->links_.begin() + i);
        return;
      }
    }

    throw std::logic_error("err");
  }

  void comMind(std::istream & in, std::ostream & out, db & data)
  {
    std::string name;
    if (!(in >> name))
    {
      throw std::logic_error("err");
    }

    std::shared_ptr< Note > note = findNote(name, data);

    for (size_t i = 0; i < note->links_.size(); ++i)
    {
      std::shared_ptr< Note > ptr = note->links_[i].lock();
      if (ptr)
      {
        out << ptr->name_ << '\n';
      }
    }
  }

  void comExpired(std::istream & in, std::ostream & out, db & data)
  {
    std::string name;
    if (!(in >> name))
    {
      throw std::logic_error("err");
    }

    std::shared_ptr< Note > note = findNote(name, data);

    size_t count = 0;
    for (size_t i = 0; i < note->links_.size(); ++i)
    {
      if (note->links_[i].expired())
      {
        ++count;
      }
    }

    out << count << '\n';
  }

  void comRefresh(std::istream & in, std::ostream &, db & data)
  {
    std::string name;
    if (!(in >> name))
    {
      throw std::logic_error("err");
    }

    std::shared_ptr< Note > note = findNote(name, data);

    size_t i = 0;
    while (i < note->links_.size())
    {
      if (note->links_[i].expired())
      {
        note->links_.erase(note->links_.begin() + i);
      }
      else
      {
        ++i;
      }
    }
  }
}