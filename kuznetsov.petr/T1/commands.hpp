#ifndef COMANDS_HPP
#define COMANDS_HPP
#include <unordered_map>
#include "record.hpp"

namespace kuznetsov {
  using notepad_t = std::unordered_map< std::string, std::shared_ptr< Record > >;

  void addNote(std::istream& in, std::ostream& , notepad_t& db);

  void addLine(std::istream& in, std::ostream&, notepad_t& db);

  void show(std::istream& in, std::ostream& out, const notepad_t& db);

  void drop(std::istream& in, std::ostream&, notepad_t& db);

  void linkNotes(std::istream& in, std::ostream&, notepad_t& db);

  void showLinks(std::istream& in, std::ostream& out, const notepad_t& db);

  void removeLink(std::istream& in, std::ostream&, notepad_t& db);

  void getCountRemovedNotes(std::istream& in, std::ostream& out, const notepad_t& db);

  void removeRemovedLinks(std::istream& in, std::ostream& out, notepad_t& db);
}

#endif
