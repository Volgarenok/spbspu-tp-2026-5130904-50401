#include "note.hpp"
#include <iostream>

namespace sedov
{
  Note::Note(const std::string & str):
    name_(str)
  {}

  void Note::nLine(const std::string & l)
  {
    lines_.push_back(l);
  }

  void Note::nShow() const
  {
    if (lines_.empty())
    {
      std::cout << "\n";
    }
    for (size_t i = 0; i < lines_.size(); ++i)
    {
      std::cout << lines_[i] << "\n";
    }
  }

  void Note::nLink(const std::weak_ptr< Note > & l)
  {
    std::shared_ptr< Note > shPtr1 = l.lock();
    for (auto it = links_.begin(); it != links_.end(); ++it)
    {
      if (auto shPtr2 = it->lock())
      {
        if (shPtr1 && shPtr2 && shPtr1 == shPtr2)
        {
          throw std::logic_error("");
        }
      }
    }
    links_.push_back(l);
  }

  void Note::nHalt(const std::shared_ptr< Note > & t)
  {
    auto it = links_.begin();
    while (it != links_.end())
    {
      if (auto shPtr = it->lock())
      {
        if (shPtr == t)
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

  void Note::nMind() const
  {
    bool p = false;
    for (auto it = links_.begin(); it != links_.end(); ++it)
    {
      if (auto shPtr = it->lock())
      {
        std::cout << shPtr->getName() << "\n";
        p = true;
      }
    }
    if (!p)
    {
      std::cout << "\n";
    }
  }

  size_t Note::nExpired() const
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

  void Note::nRefresh()
  {
    auto it = links_.begin();
    while (it != links_.end())
    {
      if (it->lock())
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
