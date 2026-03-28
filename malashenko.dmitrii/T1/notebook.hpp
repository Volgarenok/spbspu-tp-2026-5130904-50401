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
  struct Note
  {
    Note(std::string name);

    std::string name_;
    std::vector< std::string > text_;
    std::unordered_map<std::string, std::weak_ptr< Note > > links_;
  };

  struct Notebook
  {
    std::unordered_map<std::string, std::shared_ptr< Note > > notes_;

    void note(std::istream&, std::ostream&, const std::string& name);
    void line(std::istream& in, std::ostream&, const std::string& name);
    void show(std::istream&, std::ostream& out, const std::string& name);
    void drop(std::istream&, std::ostream&, const std::string& name);

    void link(std::istream& in, std::ostream&, const std::string& name);
    void halt(std::istream& in, std::ostream&, const std::string& name);
    void mind(std::istream&, std::ostream& out, const std::string& name);
    // void expired(std::istream&, std::ostream&, const std::string& name);
    // void refresh(std::istream&, std::ostream&, const std::string& name);
  };
}

#endif