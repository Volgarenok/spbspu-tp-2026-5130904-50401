#include <stdexcept>
#include <iomanip>
#include "notebook.hpp"

zharov::Book::Book()
{
  cmds_["note"] = &Book::note;
  cmds_["show"] = &Book::show;
  cmds_["drop"] = &Book::drop;
  cmds_["mind"] = &Book::mind;
  cmds_["expired"] = &Book::expired;
  cmds_["refresh"] = &Book::refresh;
  cmds_["link"] = &Book::link;
  cmds_["halt"] = &Book::halt;
  cmds_["line"] = &Book::line;
}

void zharov::Book::note(std::istream & in, std::ostream &)
{
  std::string name;
  in >> name;
  for (size_t i = 0; i < notes_.size(); ++i) {
    if (notes_.at(i)->name_ == name){
      return;
    }
  }
  std::shared_ptr< Note > new_note = std::shared_ptr< Note >(new Note);
  new_note->name_ = name;
  new_note->text_ = "";
  notes_.push_back(new_note);
}

void zharov::Book::line(std::istream & in, std::ostream &)
{
  std::string name;
  in >> name;
  std::string new_line;
  in >> std::quoted(new_line);
  for (size_t i = 0; i < notes_.size(); ++i) {
    if (notes_.at(i)->name_ == name){
      notes_[i]->text_.append(new_line + '\n');
      return;
    }
  }
  std::shared_ptr< Note > new_note = std::shared_ptr< Note >(new Note);
  new_note->name_ = name;
  new_note->text_ = new_line + '\n';
  notes_.push_back(new_note);
}

void zharov::Book::show(std::istream & in, std::ostream & out)
{
  std::string name;
  in >> name;
  for (size_t i = 0; i < notes_.size(); ++i) {
    if (notes_.at(i)->name_ == name) {
      out << notes_[i]->text_;
      return;
    }
  }
  throw std::logic_error("Note not found");
}

void zharov::Book::drop(std::istream &, std::ostream &)
{}

void zharov::Book::link(std::istream &, std::ostream &)
{}

void zharov::Book::mind(std::istream &, std::ostream &)
{}

void zharov::Book::halt(std::istream &, std::ostream &)
{}

void zharov::Book::expired(std::istream &, std::ostream &)
{}

void zharov::Book::refresh(std::istream &, std::ostream &)
{}
