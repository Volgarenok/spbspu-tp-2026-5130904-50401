#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <istream>
#include <ostream>

namespace hvostov {
  struct Note {
    std::string name;
    std::vector< std::string > lines;
    std::vector< std::weak_ptr< Note > > linked_notes;

    Note(const std::string& n):
      name(n)
    {}
  };

  using NoteBook = std::unordered_map< std::string, std::shared_ptr< Note > >;

  void addNote(std::istream& in, std::ostream&, NoteBook& notes);
  void addLine(std::istream& in, std::ostream&, NoteBook& notes);
  void showNote(std::istream& in, std::ostream& out, NoteBook& notes);
  void dropNote(std::istream& in, std::ostream&, NoteBook& notes);
  void linkNote(std::istream& in, std::ostream&, NoteBook& notes);
  void haltNote(std::istream& in, std::ostream&, NoteBook& notes);
  void mindNote(std::istream& in, std::ostream&, NoteBook& notes);
  void expiredNote(std::istream& in, std::ostream&, NoteBook& notes);
  void refreshNote(std::istream& in, std::ostream&, NoteBook& notes);
}

#endif
