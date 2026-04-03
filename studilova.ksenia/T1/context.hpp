#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <unordered_map>
#include <memory>
#include <string>

#include "note.hpp"

namespace studilova
{
  struct Context
  {
    std::unordered_map< std::string, std::shared_ptr< Note > > notes;
  };
}

#endif
