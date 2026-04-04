#ifndef NOTEBOOK_HPP
#define NOTEBOOK_HPP
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <iomanip>
#include <iostream>

namespace petrov
{
  struct Note
  {
    Note(std::string name);

    std::unordered_map<std::string, std::weak_ptr< Note > > links_;
    std::string name_;
    std::vector< std::string > data_;
  };

  struct Notebook
  {
    void note(std::ostream&, std::istream&, const std::string&);
    void line(std::ostream&, std::istream&, const std::string&);
    void show(std::ostream&, std::istream&, const std::string&);
    void drop(std::ostream&, std::istream&, const std::string&);
    void link(std::ostream&, std::istream&, const std::string&);
    void halt(std::ostream&, std::istream&, const std::string&);
    void mind(std::ostream&, std::istream&, const std::string&);
    void expired(std::ostream&, std::istream&, const std::string&);
    void refresh(std::ostream&, std::istream&, const std::string&);

    private:
      std::unordered_map< std::string, std::shared_ptr< Note > > notes_;
  };
}
#endif
