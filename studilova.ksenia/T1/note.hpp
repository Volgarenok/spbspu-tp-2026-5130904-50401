#ifndef NODE_HPP
#define NODE_HPP

#include <string>
#include <vector>
#include <memory>

namespace studilova
{
  struct Note
  {
    std::string name;
    std::vector< std::string > lines;
    std::vector< std::weak_ptr< Note > > links;
  };
}

#endif
