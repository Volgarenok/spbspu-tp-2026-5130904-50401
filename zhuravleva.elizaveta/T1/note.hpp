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
  using db = std::unordered_map<std::string, NotePtr>;
}


#endif