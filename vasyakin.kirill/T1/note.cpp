#include "note.hpp"
#include <iostream>

#include "note.hpp"
#include <iostream>

namespace vasyakin
{
  Note::Note(const std::string& name):
    name_(name)
  {}

  void Note::noteLine(const std::string& txt)
  {
    lines_.push_back(txt);
  }

  void Note::noteShow() const
  {
    for (size_t i = 0; i < lines_.size(); ++i)
    {
      std::cout << lines_[i] << '\n';
    }
  }
  
  const std::string& Note::getName() const
  {
    return name_;
  }
}