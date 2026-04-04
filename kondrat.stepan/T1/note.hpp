#ifndef NOTE_HPP
#define NOTE_HPP

#include <memory>
#include <string>
#include <vector>

namespace kondrat
{
  struct Note
  {
    std::string name_;
    std::vector< std::string > lines_;
    std::vector< std::weak_ptr< Note > > links_;
  };
}

#endif