#ifndef NOTEBOOK_HPP
#define NOTEBOOK_HPP

#include "note.hpp"
#include <iostream>
#include <vector>
namespace zubarev
{

  class NoteBook
  {
  public:


    void note(const std::string& name);
    void line(const std::string& name, std::string& text);
    void show(const std::string& name, std::ostream& out) const;
    void drop(const std::string& name);

    void link(const std::string& noteFrom, const std::string& noteTo);
    void halt(const std::string& noteFrom, const std::string& noteTo);
    void mind(const std::string& noteFrom, std::ostream& out) const;
    void expired(const std::string& noteFrom, std::ostream& out) const;
    void refresh(const std::string& noteFrom);

  private:
    std::unordered_map< std::string, std::shared_ptr< Note > > notes_;
  };

}

#endif
