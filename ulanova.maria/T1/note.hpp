#ifndef NOTE_HPP
#define NOTE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace ulanova
{
  struct Note
  {
    std::string name;
    std::vector<std::string> lines;
    std::vector<std::weak_ptr<Note>> links;
  };
  using NotePtr = std::shared_ptr<Note>;
  using DB = std::unordered_map<std::string, NotePtr>;

  void create_note(DB& db, const std::string& name);
  void add_line(DB& db, const std::string& name, const std::string& text);
  std::vector<std::string> show(const DB& db, const std::string& name);
  void drop(DB& db, const std::string& name);

  void link(DB& db, const std::string& from, const std::string& to);
  void halt(DB& db, const std::string& from, const std::string& to);
  std::vector<std::string> mind(const DB& db, const std::string& name);
  size_t expired(const DB& db, const std::string& name);
  void refresh(DB& db, const std::string& name);
}
#endif
