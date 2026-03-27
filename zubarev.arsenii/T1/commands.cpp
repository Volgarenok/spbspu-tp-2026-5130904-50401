#include "commands.hpp"
#include <iostream>
namespace zubarev
{
  void cmd_note(std::istream& in, std::ostream&, NoteBook& notebook)
  {
    std::string name;
    in >> name;
    notebook.note(name);
  }
  void cmd_line(std::istream& in, std::ostream&, NoteBook& notebook)
  {
    std::string name, text;
    in >> name >>std::quoted(text);
    notebook.line(name, text);
  }
  void cmd_show(std::istream& in, std::ostream& out, NoteBook& notebook)
  {
    std::string name;
    in >> name;
    notebook.show(name, out);
  }
  void cmd_drop(std::istream& in, std::ostream&, NoteBook& notebook)
  {
    std::string name;
    in >> name;
    notebook.drop(name);
  }

  void cmd_link(std::istream& in, std::ostream&, NoteBook& notebook)
  {
    std::string nameFrom, nameTo;
    in >> nameFrom >> nameTo;
    notebook.link(nameFrom, nameTo);
  }

  void cmd_halt(std::istream& in, std::ostream&, NoteBook& notebook)
  {
    std::string nameFrom, nameTo;
    in >> nameFrom >> nameTo;
    notebook.halt(nameFrom, nameTo);
  }
  void cmd_mind(std::istream& in, std::ostream& out, NoteBook& notebook)
  {
    std::string nameFrom;
    in >> nameFrom;
    notebook.mind(nameFrom, out);
  }
  void cmd_expired(std::istream& in, std::ostream& out, NoteBook& notebook)
  {
    std::string nameFrom;
    in >> nameFrom;
    notebook.expired(nameFrom, out);
  }
  void cmd_refresh(std::istream& in, std::ostream&, NoteBook& notebook)
  {
    std::string nameFrom;
    in >> nameFrom;
    notebook.refresh(nameFrom);
  }
}
