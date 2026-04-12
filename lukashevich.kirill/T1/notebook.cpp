#include "notebook.hpp"

lukashevich::Note::Note(std::string name):
  name_(name)
{}

void lukashevich::Notebook::note(std::istream&, std::ostream&, const std::string& name)
{
  for (size_t i = 0; i < notes_.size(); ++i)
  {
    if (notes_[i].name_ == name)
    {
      throw std::logic_error("this name already exists");
    }
  }

  Note newNote = Note(name);
  notes_.push_back(newNote);
}

void lukashevich::Notebook::line(std::istream& in, std::ostream&, const std::string& name)
{
  for (size_t i = 0; i < notes_.size(); ++i)
  {
    if (notes_[i].name_ == name)
    {
      std::string newLine;
      in >> std::quoted(newLine);
      notes_[i].text_.push_back(newLine);
      return;
    }
  }
  throw std::logic_error("there is no note with this name");
}

void lukashevich::Notebook::show(std::istream&, std::ostream& out, const std::string& name)
{
  for (size_t i = 0; i < notes_.size(); ++i)
  {
    if (notes_[i].name_ == name)
    {
      for (size_t j = 0; j < notes_[i].text_.size(); ++j)
      {
        out << notes_[i].text_[j] << '\n';
      }
      return;
    }
  }
  throw std::logic_error("there is no note with this name");
}

void lukashevich::Notebook::drop(std::istream&, std::ostream&, const std::string& name)
{
  for (size_t i = 0; i < notes_.size(); ++i)
  {
    if (notes_[i].name_ == name)
    {
      notes_.erase(notes_.begin() + i);
      return;
    }
  }
  throw std::logic_error("there is no note with this name");
}
