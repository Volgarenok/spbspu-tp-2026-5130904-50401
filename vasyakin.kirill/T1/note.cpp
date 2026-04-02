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

  void Note::noteLink(const std::weak_ptr< Note >& link)
  {
    std::shared_ptr< Note > shr_ptr1 = link.lock();
    for (auto it = links_.begin(); it != links_.end(); ++it)
    {
      if (auto shr_ptr2 = it->lock())
      {
        if (shr_ptr1 && shr_ptr2 && shr_ptr1 == shr_ptr2)
        {
          throw std::logic_error("");
        }
      }
    }
    links_.push_back(link);
  }

  void Note::noteHalt(const std::shared_ptr< Note >& target)
  {
    if (target.get() == this)
    {
      throw std::logic_error("");
    }

    auto it = links_.begin();
    while (it != links_.end())
    {
      if (auto shr_ptr = it->lock())
      {
        if (shr_ptr == target)
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

  void Note::noteMind() const
  {
    for (auto it = links_.begin(); it != links_.end(); ++it)
    {
      if (auto shr_ptr = it->lock())
      {
        std::cout << shr_ptr->getName() << '\n';
      }
    }
  }

  size_t Note::noteExpired() const
  {
    size_t count = 0;
    for (auto it = links_.begin(); it != links_.end(); ++it)
    {
      if (!it->lock())
      {
        ++count;
      }
    }
    return count;
  }

  void Note::noteRefresh()
  {
    auto it = links_.begin();
    while (it != links_.end())
    {
      if (auto shr_ptr = it->lock())
      {
        ++it;
      }
      else
      {
        it = links_.erase(it);
      }
    }
  }

  const std::string& Note::getName() const
  {
    return name_;
  }
}
