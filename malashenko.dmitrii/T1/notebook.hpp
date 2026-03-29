#ifndef NOTEBOOK
#define NOTEBOOK

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <iomanip>
#include <unordered_map>

namespace malashenko
{
  struct Note {
    Note(std::string name);

    std::string name_;
    std::vector< std::string > text_;
    std::unordered_map<std::string, std::weak_ptr< Note > > links_;
  };

  class Notebook {
  public:
    void note(std::istream&, std::ostream&, const std::string&);
    void line(std::istream&, std::ostream&, const std::string&);
    void show(std::istream&, std::ostream&, const std::string&);
    void drop(std::istream&, std::ostream&, const std::string&);

    void link(std::istream&, std::ostream&, const std::string&);
    void halt(std::istream&, std::ostream&, const std::string&);
    void mind(std::istream&, std::ostream&, const std::string&);
    void expired(std::istream&, std::ostream&, const std::string&);
    void refresh(std::istream&, std::ostream&, const std::string&);
  private:
    std::unordered_map<std::string, std::shared_ptr< Note > > notes_;
  };
}

#endif