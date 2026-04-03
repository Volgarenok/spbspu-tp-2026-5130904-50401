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

  void Note::showLines() const
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
