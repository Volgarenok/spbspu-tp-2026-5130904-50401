#include "notebook.hpp"

petrov::Note::Note(std::string name):
  name_(name)
{}

void petrov::Notebook::note(std::ostream& out, std::istream& in, const std::string& t)
{
  try
  {
    notes_.at(t);
    throw std::logic_error("Try another name for Note");
  }
  catch(std::out_of_range &)
  {
    notes_[t] = std::shared_ptr< Note >(new Note(t));
  }
}

void petrov::Notebook::line(std::ostream& out, std::istream& in, const std::string& t)
{
  try
  {
    std::string text;
    in >> text;
    notes_.at(t).get()->data_.push_back(text);
  }
  catch(std::out_of_range&)
  {
    throw std::logic_error("No Note with this name :(");
  }
}

void petrov::Notebook::show(std::ostream& out, std::istream& in, const std::string& t)
{
  try
  {
    auto data = notes_.at(t).get()->data_;
    if (data.size())
    {
      for (size_t i = 0; i < data.size(); ++i)
      {
        out << data[i] << "\n";
      }
    }
    else
    {
      out << "\n";
    }
  }
  catch(std::out_of_range&)
  {
    throw std::logic_error("No Note with this name :(");
  }
}

void petrov::Notebook::drop(std::ostream& out, std::istream& in, const std::string& t)
{
  try
  {
    notes_.erase(t);
  }
  catch(std::out_of_range&)
  {
    throw std::logic_error("No Note with this name :(");
  }
}

void petrov::Notebook::link(std::ostream& out, std::istream& in, const std::string& t)
{
  try
  {
    std::string toNote;
    in >> toNote;
    try
    {
      notes_.at(t).get()->links_.at(toNote);
      throw std::logic_error("This link in Note now :(");
    }
    catch(std::out_of_range &)
    {}

    auto toNotePtr = notes_.at(toNote);
    notes_.at(t).get()->links_[toNote] = toNotePtr;
  }
  catch(std::out_of_range&)
  {
    throw std::logic_error("No Note with this name :(");
  }
}

void petrov::Notebook::halt(std::ostream& out, std::istream& in, const std::string& t)
{
  try
  {
    std::string toNote;
    in >> toNote;
    try
    {
      notes_.at(t).get()->links_.at(toNote);
    }
    catch(std::out_of_range &)
    {
      throw std::logic_error("No Link in this Note :(");
    }
    notes_.at(t).get()->links_.erase(toNote);
  }
  catch(std::out_of_range&)
  {
    throw std::logic_error("No Note with this name :(");
  }
}

void petrov::Notebook::mind(std::ostream& out, std::istream& in, const std::string& t)
{
  
}
