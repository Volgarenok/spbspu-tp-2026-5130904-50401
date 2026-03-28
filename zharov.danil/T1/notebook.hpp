#ifndef NOTEBOOK_HPP
#define NOTEBOOK_HPP
#include <string>
#include <memory>
#include <vector>

namespace zharov {
  struct Note {
    std::string name_;
    std::string text_;
    std::vector < std::weak_ptr < Note > > links_;
  };
  
}

#endif
