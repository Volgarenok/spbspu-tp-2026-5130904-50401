#ifndef NOTE_HPP
#define NOTE_HPP

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

namespace khasnulin
{
  struct Note
  {
    Note(const std::string &Name):
        name(Name)
    {
    }

    std::string name;
    std::vector< std::string > lines;
    std::vector< std::weak_ptr< Note > > links;
    std::unordered_set< std::string > links_names;
  };
}

#endif
