#include "note.hpp"
#include <stdexcept>

namespace ulanova
{
  void create_note(DB& db, const std::string& name)
  {
    if (db.count(name))
    {
      throw std::logic_error("error");
    }
    db[name] = std::make_shared<Note>(Note{name});
  }
  void add_line(DB& db, const std::string& name, const std::string& text)
  {
    auto it = db.find(name);
    if (it == db.end())
    {
      throw std::logic_error("note not found");
    }
    it -> second -> lines.push_back(text);
  }
  std::vector<std::string> show(const DB& db, const std::string& name)
  {
    auto it = db.find(name);
    if (it == db.end())
    {
      throw std::logic_error("note not found");
    }
    return it->second->lines;
  }
  void drop(DB& db, const std::string& name)
  {
    if (db.find(name) == db.end())
    {
      throw std::logic_error("error");
    }
    db.erase(name);
  }
  void link(DB& db, const std::string& from, const std::string& to)
  {
    auto it_from = db.find(from);
    auto it_to = db.find(to);
    if (it_from == db.end() || it_to == db.end())
    {
      throw std::logic_error("note not found");
    }
    auto& links = it_from -> second -> links;
    for(size_t i = 0; i < links.size(); ++i)
    {
      auto ptr = links[i].lock();
      if (ptr != nullptr)
      {
        if (ptr == it_to -> second)
        {
          throw std::logic_error("error");
        }
      }
    }
    links.push_back(it_to -> second);
  }
  std::vector<std::string> mind(const DB& db, const std::string& name)
  {
    auto it = db.find(name);
    if (it == db.end())
    {
      throw std::logic_error("note not found");
    }
    std::vector<std::string> results;
    auto& links = it -> second -> links;
    for (size_t i = 0; i< links.size();++i)
    {
      auto ptr = links[i].lock();
      if (ptr != nullptr)
      {
        results.push_back(ptr -> name);
      }
    }
    return results;
  }
  void halt(DB& db, const std::string& from, const std::string& to)
  {
    auto it_from = db.find(from);
    auto it_to = db.find(to);
    if(it_from == db.end() || it_to == db.end())
    {
      throw std::logic_error("note not found");
    }
    auto& links = it_from -> second -> links;
    bool found = false;
    for (auto it = links.begin(); it != links.end(); ++it)
    {
      auto ptr = it->lock();
      if (ptr == nullptr)
      {
        continue;
      }
      if (ptr == it_to->second)
      {
        links.erase(it);
        found = true;
        break;
      }
    }
    if (!found)
    {
      throw std::logic_error("link not found");
    }
  }
  size_t expired(const DB& db, const std::string& name)
  {
    auto it = db.find(name);
    if (it == db.end())
    {
      throw std::logic_error("note not found");
    }
    size_t count = 0;
    auto& links = it -> second -> links;
    for (size_t i = 0; i < links.size(); i++)
    {
      if (links[i].lock() == nullptr)
      {
        count ++;
      }
    }
    return count;
  }
  void refresh(DB& db, const std::string& name)
  {
    auto it = db.find(name);
    if ( it == db.end())
    {
      throw std::logic_error("error");
    }
    auto& links = it -> second -> links;
    for (auto it_link = links.begin(); it_link != links.end();)
    {
      if (it_link->lock() == nullptr)
      {
        it_link = links.erase(it_link);
      }
      else
      {
        ++it_link;
      }
    }
  }
}
