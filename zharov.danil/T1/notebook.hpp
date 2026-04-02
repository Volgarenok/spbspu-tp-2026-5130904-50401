#ifndef NOTEBOOK_HPP
#define NOTEBOOK_HPP
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

namespace zharov {
  struct Note {
    std::string name_;
    std::string text_;
    std::vector < std::weak_ptr < Note > > links_;
  };

  struct Book {
    Book();
    using cmd_t = void(Book::*)(std::istream &, std::ostream &);
    std::unordered_map< std::string, cmd_t > cmds_;
  private:
    std::vector < std::shared_ptr< Note > > notes_;
    void note(std::istream & in, std::ostream & out);
    void line(std::istream & in, std::ostream & out);
    void show(std::istream & in, std::ostream & out);
    void drop(std::istream & in, std::ostream & out);
    void link(std::istream & in, std::ostream & out);
    void mind(std::istream & in, std::ostream & out);
    void halt(std::istream & in, std::ostream & out);
    void expired(std::istream & in, std::ostream & out);
    void refresh(std::istream & in, std::ostream & out);
  };

}

#endif
