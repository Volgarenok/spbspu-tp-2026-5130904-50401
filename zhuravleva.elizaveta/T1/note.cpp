#include "note.hpp"
#include <stdexcept>

namespace zhuravleva
{
  void create_note(Db& db, const std::string& name)
  {
    if(db.count(name))
    {
      throw std::logic_error("note with this name already exists");
    }
    NotePtr note = std::make_shared< Note >();
    note -> name = name;
    db[name] = note;
  }

  void add_line(Db& db, const std::string& name, const std::string& text)
  {
    if(!db.count(name))
    {
      throw std::logic_error("note is hasn't been found");
    }
    db[name] -> lines.push_back(text);
  }

  std::vector<std::string> show(const Db& db, const std::string& name)
  {
    if(!db.count(name))
    {
      throw std::logic_error("note is hasn't been found");
    }
    return db.at(name) -> lines;
  }

  void drop(Db& db, std::string& name)
  {
    if(!db.count(name))
    {
      throw std::logic_error("note is hasn't been found");
    }
    db.erase(name);
  }


  void link(Db& db, const std::string& from, const std::string& to)
  {
    if (!db.count(from) || !db.count(to))
    {
      throw std::logic_error("invalid link");
    }
    auto from_note = db[from];
    auto to_note = db[to];
    for(size_t i = 0; i < from_note -> links.size(); i++)
    {
      auto sp = from_note -> links[i].lock();
      if (sp && sp -> name == to)
      {
        throw std::logic_error("try to duplicate link");
      }
    }
    from_note->links.push_back(to_note);
  }

  void halt(Db& db, const std::string& from, const std::string& to)
  {
    
    if (!db.count(from) || !db.count(to))
    {
      throw std::logic_error("invalid link");
    }
    auto from_note = db[from];
    auto to_note = db[to];
    for (size_t i = 0; i < from_note->links.size(); ++i)
    {
      auto sp = from_note->links[i].lock();
      if (sp && sp->name == to)
      {
        for (size_t j = i; j + 1 < from_note->links.size(); ++j)
        {
          from_note->links[j] = from_note->links[j + 1];
        }
        from_note->links.pop_back();
        return;
      }
    }
    throw std::logic_error("link hasn't been found");
  }

  std::vector<std::string> mind(Db& db, const std::string& name)
  {
    if (!db.count(name))
    {
      throw std::logic_error("note not found");
    }
    std::vector< std::string > results;
    auto note = db.at(name);
    for (size_t i = 0; i < note -> links.size(); i++)
    {
      auto sp = note -> links[i].lock();
      if (sp)
      {
        results.push_back(sp -> name);
      }
    }
    return results;
  }

  size_t expired(const Db& db, const std::string& name)
  {
    if (!db.count(name))
    {
      throw std::logic_error("note not found");
    }
    size_t count = 0;
    auto note = db.at(name);
    for (size_t i = 0; i < note -> links.size(); i++)
    {
      if (note -> links[i].expired())
      {
        count++;
      }
    }
    return count;
  }

  void refresh(Db& db, const std::string& name)
  {
    if (!db.count(name))
    {
      throw std::logic_error("note not found");
    }
    auto note = db[name];
    size_t i = 0;
    while (i < note -> links.size())
    {
      if (note -> links[i].expired())
      {
        for (size_t j = i; j + 1 < note -> links.size(); ++j)
        {
          note->links[j] = note -> links[j + 1];
        }
        note -> links.pop_back();
      } else
      {
        i++;
      }
    }
  }
}
