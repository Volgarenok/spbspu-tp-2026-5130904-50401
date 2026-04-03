#include "note.hpp"

#include <iostream>

namespace burukov
{
  Note::Note(const std::string& name):
    name_(name)
  {}

  void Note::addLine(const std::string& text)
  {
    lines_.push_back(text);
  }

  const std::string& Note::getName() const
  {
    return name_;
  }
}
