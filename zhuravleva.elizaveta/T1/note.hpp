#ifndef NOTE_HPP
#define NOTE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace zhuravleva
{
  struct Note
  {
    std::string name;
    std::vector<std::string> lines;
    std::vector<std::weak_ptr<Note>> links;
  };
  using NotePtr = std::shared_ptr<Note>;
  using Db = std::unordered_map<std::string, NotePtr>;

  void create_note(Db& db, const std::string& name);
  void add_line(Db& db, const std::string& name, const std::string& text);
  std::vector<std::string> show(const Db& db, const std::string& name);
  void drop(Db& db, std::string& name);

  void link(Db& db, const std::string& from, const std::string& to);
  void halt(Db& db, const std::string& from, const std::string& to);
  std::vector<std::string> mind(Db& db, const std::string& name);
  size_t expired(const Db& db, const std::string& name);
  void refresh(Db& db, const std::string& name);
}



#endif
