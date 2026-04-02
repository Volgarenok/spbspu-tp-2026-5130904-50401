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

bool zharov::Book::isInNotes(size_t & ind, const std::string & name)
{
  for (size_t i = 0; i < notes_.size(); ++i) {
    if (notes_.at(i)->name_ == name){
      ind = i;
      return true;
    }
  }
  return false;
}

bool zharov::Book::isInNotes(const std::string & name)
{
  size_t ind = 0;
  return isInNotes(ind, name);
}

void zharov::Book::note(std::istream & in, std::ostream &)
{
  std::string name;
  in >> name;
  if (isInNotes(name)) {
    return;
  }
  std::shared_ptr< Note > new_note = std::make_shared< Note >(name);
  notes_.push_back(new_note);
}

void zharov::Book::line(std::istream & in, std::ostream &)
{
  std::string name;
  std::string new_line;
  in >> name >> std::quoted(new_line);
  size_t ind = 0;
  if (isInNotes(ind, name)) {
    notes_.at(ind)->text_.append(new_line + '\n');
    return;
  }
  throw std::logic_error("Note not found");
}

void zharov::Book::show(std::istream & in, std::ostream & out)
{
  std::string name;
  in >> name;
  size_t ind = 0;
  if (isInNotes(ind, name)) {
    if (notes_.at(ind)->text_ == "") {
      out << "\n";
    }
    out << notes_.at(ind)->text_;
    return;
  }
  throw std::logic_error("Note not found");
}

void zharov::Book::drop(std::istream & in, std::ostream &)
{
  std::string name;
  in >> name;
  size_t ind = 0;
  if (!isInNotes(ind, name)) {
    throw std::logic_error("Note not found");
  }
  notes_.erase(notes_.begin() + ind);
}

void zharov::Book::link(std::istream & in, std::ostream &)
{
  std::string name_from;
  std::string name_to;
  in >> name_from >> name_to;
  size_t ind_from = 0;
  size_t ind_to = 0;
  if (!(isInNotes(ind_from, name_from) && isInNotes(ind_to, name_to))) {
    throw std::logic_error("Note not found");
  }
  for (size_t i = 0; i < notes_.at(ind_from)->links_.size(); ++i) {
    if (notes_.at(ind_from)->links_.at(i).lock() == notes_.at(ind_to)) {
      throw std::logic_error("Already linked");
    }
  }
  notes_.at(ind_from)->links_.push_back(notes_.at(ind_to));
}

void zharov::Book::mind(std::istream & in, std::ostream & out)
{
  std::string name;
  in >> name;
  size_t ind = 0;
  if (!isInNotes(ind, name)) {
    throw std::logic_error("Note not found");
  }
  for (size_t i = 0; i < notes_.at(ind)->links_.size(); ++i) {
    if (auto lnk = notes_.at(ind)->links_.at(i).lock()) {
      out << lnk->name_ << '\n';
    }
  }
}

void zharov::Book::halt(std::istream & in, std::ostream &)
{
  std::string name_from;
  std::string name_to;
  in >> name_from >> name_to;
  size_t ind_from = 0;
  size_t ind_to = 0;
  if (!(isInNotes(ind_from, name_from) && isInNotes(ind_to, name_to))) {
    throw std::logic_error("Note not found");
  }
  for (size_t i = 0; i < notes_.at(ind_from)->links_.size(); ++i) {
    if (notes_.at(ind_from)->links_.at(i).lock() == notes_.at(ind_to)) {
      notes_.at(ind_from)->links_.erase(notes_.at(ind_from)->links_.begin() + i);
      return;
    }
  }
  throw std::logic_error("Link not found");
}

void zharov::Book::expired(std::istream & in, std::ostream & out)
{
  std::string name;
  in >> name;
  size_t ind = 0;
  if (!isInNotes(ind, name)) {
    throw std::logic_error("Note not found");
  }
  size_t count = 0;
  for (size_t i = 0; i < notes_.at(ind)->links_.size(); ++i) {
    if (!notes_.at(ind)->links_.at(i).lock()) {
      ++count;
    }
  }
  out << count << '\n';
}

void zharov::Book::refresh(std::istream & in, std::ostream &)
{
  std::string name;
  in >> name;
  size_t ind = 0;
  if (!isInNotes(ind, name)) {
    throw std::logic_error("Note not found");
  }
  size_t count = 0;
  for (size_t i = 0; i < notes_.at(ind)->links_.size(); ++i) {
    if (!notes_.at(ind)->links_.at(i).lock()) {
      ++count;
    }
  }
  for (size_t j = 0; j < count; ++j) {
    for (size_t i = 0; i < notes_.at(ind)->links_.size(); ++i) {
      if (!notes_.at(ind)->links_.at(i).lock()) {
        notes_.at(ind)->links_.erase(notes_.at(ind)->links_.begin() + i);
        break;
      }
    }
  }
}
