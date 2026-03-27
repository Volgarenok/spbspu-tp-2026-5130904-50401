#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <istream>
#include <ostream>

#include "context.hpp"

namespace studilova
{
  using cmd_t = void(*)(std::istream&, std::ostream&, Context&);

  void note(std::istream&, std::ostream&, Context&);
  void line(std::istream&, std::ostream&, Context&);
  void show(std::istream&, std::ostream&, Context&);
  void drop(std::istream&, std::ostream&, Context&);

  void link(std::istream&, std::ostream&, Context&);
  void halt(std::istream&, std::ostream&, Context&);
  void mind(std::istream&, std::ostream&, Context&);
  void expired(std::istream&, std::ostream&, Context&);
  void refresh(std::istream&, std::ostream&, Context&);
}

#endif
