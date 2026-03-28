#include "notebook.hpp"

malashenko::Note::Note(std::string name):
  name_(name)
{}

void malashenko::Notebook::note(std::istream&, std::ostream&, const std::string& name)
{
  try
  {
    notes_.at(name);
    throw std::logic_error("You already have note with this name");
  } catch(std::out_of_range &)
  {
    notes_[name] = std::shared_ptr< Note >(new Note(name));
  }
}

void malashenko::Notebook::line(std::istream& in, std::ostream&, const std::string& name)
{
  try
  {
    std::string newLine;
    in >> std::quoted(newLine);
    notes_.at(name).get()->text_.push_back(newLine);
  } catch(std::out_of_range &)
  {
    throw std::logic_error("You don't have note with this name");
  }
}

void malashenko::Notebook::show(std::istream&, std::ostream& out, const std::string& name)
{
  try
  {
    auto text = notes_.at(name).get()->text_;
    for (size_t i = 0; i < text.size(); ++i)
    {
      out << text[i] << '\n';
    }
  } catch(std::out_of_range &)
  {
    throw std::logic_error("You don't have note with this name");
  }
}

void malashenko::Notebook::drop(std::istream&, std::ostream&, const std::string& name)
{
  try
  {
    notes_.erase(name);
  } catch(std::out_of_range &)
  {
    throw std::logic_error("You don't have note with this name");
  }
}

void malashenko::Notebook::link(std::istream& in, std::ostream&, const std::string& name)
{
  try
  {
    std::string anotherNoteName;
    in >> anotherNoteName;
    auto currNoteLinks = notes_.at(name).get()->links_;
    try
    {
      currNoteLinks.at(anotherNoteName);
      throw std::logic_error("You already have link to this note");
    }
    catch(std::out_of_range &)
    {}
    auto anotherNotePtr = notes_.at(anotherNoteName);
    currNoteLinks[anotherNoteName] = anotherNotePtr;
  }
  catch(std::out_of_range &)
  {
    throw std::logic_error("You don't have note with this name");
  }
}

void malashenko::Notebook::halt(std::istream& in, std::ostream&, const std::string& name)
{
  try
  {
    std::string noteToRemove;
    in >> noteToRemove;
    auto currNoteLinks = notes_.at(name).get()->links_;
    try
    {
      currNoteLinks.at(noteToRemove);
    }
    catch(std::out_of_range &)
    {
      throw std::logic_error("There is no link to this note");
    }
    currNoteLinks.erase(noteToRemove);
  }
  catch(std::out_of_range &)
  {
    throw std::logic_error("You don't have note with this name");
  }
}

void malashenko::Notebook::mind(std::istream&, std::ostream& out, const std::string& name)
{
  try
  {
    auto currNoteLinks = notes_.at(name).get()->links_;
    for (auto i = currNoteLinks.begin(); i != currNoteLinks.end(); ++i)
    {
      out << i->first << '\n';
    }
  }
  catch(std::out_of_range &)
  {
    throw std::logic_error("You don't have note with this name");
  }
}





