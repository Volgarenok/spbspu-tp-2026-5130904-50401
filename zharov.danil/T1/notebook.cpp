#include <stdexcept>
#include "notebook.hpp"

zharov::Book::Book()
{
  cmds_["note"] = note;
  cmds_["show"] = show;
  cmds_["drop"] = drop;
  cmds_["mind"] = mind;
  cmds_["expired"] = expired;
  cmds_["refresh"] = refresh;
  cmds_["link"] = link;
  cmds_["halt"] = halt;
  cmds_["line"] = line;
}

void zharov::Book::note(std::istream & in, std::ostream & out)
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

void zharov::Book::line(std::istream & in, std::ostream & out)
{
  std::string name;
  in >> name;
  std::string new_line;
  in >> new_line;
  for (size_t i = 0; i < notes_.size(); ++i) {
    if (notes_.at(i)->name_ == name){
      notes_[i]->text_.append(new_line + '\n');
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
    }
  }
  throw std::logic_error("Note not found");
}

void zharov::Book::drop(std::istream & in, std::ostream & out)
{}

void zharov::Book::link(std::istream & in, std::ostream & out)
{}

void zharov::Book::mind(std::istream & in, std::ostream & out)
{}

void zharov::Book::halt(std::istream & in, std::ostream & out)
{}


void zharov::Book::expired(std::istream & in, std::ostream & out)
{}

void zharov::Book::refresh(std::istream & in, std::ostream & out)
{}
