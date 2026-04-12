#include "notebook.hpp"

lukashevich::Note::Note(std::string name):
  name_(name)
{}

void lukashevich::Notebook::note(std::istream&, std::ostream&, const std::string& name)
{
  try
  {
    notes_.at(name);
    throw std::logic_error("this name already exists");
  }
  catch(std::out_of_range &)
  {
    notes_[name] = std::shared_ptr< Note >(new Note(name));
  }
}

void lukashevich::Notebook::line(std::istream& in, std::ostream&, const std::string& name)
{
  try
  {
    std::string newLine;
    in >> std::quoted(newLine);
    notes_.at(name).get()->text_.push_back(newLine);
  }
  catch(std::out_of_range &)
  {
    throw std::logic_error("there is no note with this name");
  }
}

void lukashevich::Notebook::show(std::istream&, std::ostream& out, const std::string& name)
{
  try
  {
    auto text = notes_.at(name).get()->text_;
    if( text.size())
    {
      for (size_t i = 0; i < text.size(); ++i)
      {
        out << text[i] << '\n';
      }
    }
  }
  catch(std::out_of_range &)
  {
    throw std::logic_error("there is no note with this name");
  }
}

void lukashevich::Notebook::drop(std::istream&, std::ostream&, const std::string& name)
{
   try
  {
    notes_.at(name);
    notes_.erase(name);
  } catch(std::out_of_range &)
  {
    throw std::logic_error("there is no note with this name");
  }
}

void lukashevich::Notebook::link(std::istream& in, std::ostream&, const std::string& name)
{
  try
  {
    std::string anotherNoteName;
    in >> anotherNoteName;
    try
    {
      notes_.at(name).get()->links_.at(anotherNoteName);
      throw std::logic_error("there is already link to this note");
    }
    catch(std::out_of_range &)
    {}
    auto anotherNotePtr = notes_.at(anotherNoteName);
    notes_.at(name).get()->links_[anotherNoteName] = anotherNotePtr;
  }
  catch(std::out_of_range &)
  {
    throw std::logic_error("there is no note with this name");
  }
}

void lukashevich::Notebook::halt(std::istream& in, std::ostream&, const std::string& name)
{
  try
  {
    std::string noteToRemove;
    in >> noteToRemove;
    try
    {
      notes_.at(name).get()->links_.at(noteToRemove);
    }
    catch(std::out_of_range &)
    {
      throw std::logic_error("there is no link to this note");
    }
    notes_.at(name).get()->links_.erase(noteToRemove);
  }
  catch(std::out_of_range &)
  {
    throw std::logic_error("there is no note with this name");
  }
}

void lukashevich::Notebook::mind(std::istream&, std::ostream& out, const std::string& name)
{
  try
  {
    auto currNoteLinks = notes_.at(name).get()->links_;
    std::vector< std::string > resVec;
    for (auto i = currNoteLinks.begin(); i != currNoteLinks.end(); ++i)
    {
      if (i->second.lock().get())
      {
        resVec.push_back(i->first);
      }
    }

    if(resVec.size())
    {
      for (size_t i = 0; i < resVec.size(); ++i)
      {
        out << resVec[resVec.size() - 1 - i] << '\n';
      }
    }
  }
  catch(std::out_of_range &)
  {
    throw std::logic_error("there is no note with this name");
  }
}

void lukashevich::Notebook::expired(std::istream&, std::ostream& out, const std::string& name)
{
  try
  {
    auto currLinks = notes_.at(name).get()->links_;
    size_t counter = 0;
    for (auto i = currLinks.begin(); i != currLinks.end(); ++i)
    {
      if (!(i->second).lock().get())
      {
        counter++;
      }
    }
    out << counter << '\n';
  }
  catch(std::out_of_range &)
  {
    throw std::logic_error("there is no note with this name");
  }
}

void lukashevich::Notebook::refresh(std::istream&, std::ostream&, const std::string& name)
{
  try
  {
    auto currLinks = notes_.at(name).get()->links_;
    for (auto i = currLinks.begin(); i != currLinks.end(); ++i)
    {
      if (!(i->second).lock().get())
      {
        notes_.at(name).get()->links_.erase(i->first);
      }
    }
  }
  catch(std::out_of_range &)
  {
    throw std::logic_error("there is no note with this name");
  }
}
