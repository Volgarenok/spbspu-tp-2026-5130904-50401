#include <iostream>
#include <iomanip>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace chernov {
  class Note {
  public:
    Note(std::string name);
    std::string getName() const noexcept;
    std::string getText() const noexcept;
    void addLine(std::string line);
    void addLink(std::weak_ptr< Note > new_link);
  private:
    std::string name_;
    std::string text_;
    std::vector< std::weak_ptr< Note > > links_;
  };

  class NoteBook {
  public:
    void createNote(std::string name);
    void dropNote(std::string name);
    void addLineToNote(std::string name, std::string line);
    void showNote(std::string name, std::ostream & output);
    void linkNotes(std::string name_from, std::string name_to);
  private:
    std::unordered_map< std::string, std::shared_ptr< Note > > notes_;
  };

  void cmdNote(std::istream & input, std::ostream & output, NoteBook & notebook);
  void cmdLine(std::istream & input, std::ostream & output, NoteBook & notebook);
  void cmdShow(std::istream & input, std::ostream & output, NoteBook & notebook);
  void cmdDrop(std::istream & input, std::ostream & output, NoteBook & notebook);
  void cmdLink(std::istream & input, std::ostream & output, NoteBook & notebook);
}

int main()
{
  using namespace chernov;
  using cmd_t = void(*)(std::istream &, std::ostream &, NoteBook &);
  std::istream & input = std::cin;
  std::ostream & output = std::cout;

  NoteBook notebook;

  std::unordered_map< std::string, cmd_t > cmds;
  cmds["note"] = cmdNote;
  cmds["line"] = cmdLine;
  cmds["show"] = cmdShow;
  cmds["drop"] = cmdDrop;
  cmds["link"] = cmdLink;

  std::string cmd;
  while (input >> cmd) {
    try {
      cmds.at(cmd)(input, output, notebook);
    } catch (const std::out_of_range & e) {
      output << "< INVALID COMMAND >\n";
      auto max_streamsize = std::numeric_limits< std::streamsize >::max();
      input.ignore(max_streamsize, '\n');
    } catch (const std::logic_error & e) {
      output << "< INVALID COMMAND: " << e.what() << " >\n";
    }
  }
  if (!input.eof()) {
    std::cerr << "bad input\n";
    return 1;
  }
}

chernov::Note::Note(std::string name):
  name_(name)
{}

std::string chernov::Note::getName() const noexcept
{
  return name_;
}

std::string chernov::Note::getText() const noexcept
{
  return text_;
}

void chernov::Note::addLine(std::string line)
{
  if (!text_.empty()) {
    text_ += "\n";
  }
  text_ += line;
}

void chernov::Note::addLink(std::weak_ptr< Note > new_link)
{
  for (auto link : links_) {
    if (link.lock() == new_link.lock()) {
      throw std::logic_error("link already exists");
    }
  }
  links_.push_back(new_link);
}

void chernov::NoteBook::createNote(std::string name)
{
  if (notes_.count(name)) {
    throw std::logic_error("note already exists");
  }
  notes_[name] = std::make_shared< Note >(name);
}

void chernov::NoteBook::dropNote(std::string name)
{
  if (!notes_.erase(name)) {
    throw std::logic_error("note not found");
  }
}

void chernov::NoteBook::addLineToNote(std::string name, std::string line)
{
  try {
    notes_.at(name)->addLine(line);
  } catch (const std::out_of_range & e) {
    throw std::logic_error("note not found");
  }
}

void chernov::NoteBook::showNote(std::string name, std::ostream & output)
{
  std::string text;
  try {
    text = notes_.at(name)->getText();
  } catch (const std::out_of_range & e) {
    throw std::logic_error("note not found");
  }

  output << text << "\n";
}

void chernov::NoteBook::linkNotes(std::string name_from, std::string name_to)
{
  std::weak_ptr< Note > link;
  try {
    link = std::weak_ptr< Note >(notes_.at(name_to));
  } catch (const std::out_of_range & e) {
    throw std::logic_error("note-to not found");
  }

  try {
    notes_.at(name_from)->addLink(link);
  } catch (const std::out_of_range & e) {
    throw std::logic_error("note-from not found");
  }
}

void chernov::cmdNote(std::istream & input, std::ostream &, NoteBook & notebook)
{
  std::string name;
  input >> name;
  notebook.createNote(name);
}

void chernov::cmdLine(std::istream & input, std::ostream &, NoteBook & notebook)
{
  std::string name, line;
  input >> name;
  input >> std::quoted(line);
  notebook.addLineToNote(name, line);
}

void chernov::cmdShow(std::istream & input, std::ostream & output, NoteBook & notebook)
{
  std::string name;
  input >> name;
  notebook.showNote(name, output);
}

void chernov::cmdDrop(std::istream & input, std::ostream &, NoteBook & notebook)
{
  std::string name;
  input >> name;
  notebook.dropNote(name);
}

void chernov::cmdLink(std::istream & input, std::ostream &, NoteBook & notebook)
{
  std::string name_from, name_to;
  input >> name_from;
  input >> name_to;
  notebook.linkNotes(name_from, name_to);
}
