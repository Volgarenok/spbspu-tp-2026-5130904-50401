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

  void Note::addLink(const std::weak_ptr< Note >& link)
  {
    std::shared_ptr< Note > newLink = link.lock();

    for (auto it = links_.begin(); it != links_.end(); ++it)
    {
      if (auto existing = it->lock())
      {
        if (newLink && existing && newLink == existing)
        {
          throw std::logic_error("");
        }
      }
    }

    links_.push_back(link);
  }

  void Note::removeLink(const std::shared_ptr< Note >& target)
  {
    auto it = links_.begin();

    while (it != links_.end())
    {
      if (auto current = it->lock())
      {
        if (current == target)
        {
          it = links_.erase(it);

          return;
        }
        else
        {
          ++it;
        }
      }
      else
      {
        ++it;
      }
    }
  }

  const std::string& Note::getName() const
  {
    return name_;
  }
}
