#ifndef NOTEBOOK_HPP
#define NOTEBOOK_HPP
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

namespace zharov {
  struct Note {
    Note(std::string name):
      name_(name),
      text_{},
      links_{}
    {}
    std::string name_;
    std::string text_;
    std::vector < std::weak_ptr < Note > > links_;
  };

  struct Book {
    Book();
    using cmd_t = void(Book::*)(std::istream &, std::ostream &);
    std::unordered_map< std::string, cmd_t > cmds_;
    std::vector < std::shared_ptr< Note > > notes_;
    bool isInNotes(size_t &, const std::string &);
    bool isInNotes(const std::string &);
    void note(std::istream &, std::ostream &);
    void line(std::istream &, std::ostream &);
    void show(std::istream &, std::ostream &);
    void drop(std::istream &, std::ostream &);
    void link(std::istream &, std::ostream &);
    void mind(std::istream &, std::ostream &);
    void halt(std::istream &, std::ostream &);
    void expired(std::istream &, std::ostream &);
    void refresh(std::istream &, std::ostream &);
  };

}

#endif
