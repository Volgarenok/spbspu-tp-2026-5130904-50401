#ifndef NOTE_HPP
#define NOTE_HPP

#include <iosfwd>
#include <vector>
namespace zubarev
{
  struct Note
  {
    std::string name_;
    std::vector< std::string > lines_;
    std::vector< std::weak_ptr< Note > > links_;

    Note(std::string name):
      name_(name)
    {}
  };
}

#endif
