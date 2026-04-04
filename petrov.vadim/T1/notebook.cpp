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
    throw std::logic_error("No Note woth this name :(");
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
    throw std::logic_error("No Note woth this name :(");
  }
}


