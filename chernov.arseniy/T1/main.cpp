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
    void removeLink(std::string removable_link);
    std::vector< std::string > getValidLinkNames() const;
    size_t countExpired() const noexcept;
    void cleanExpired();
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
    void mindNote(std::string name, std::ostream & output);
    void haltLink(std::string name_from, std::string name_to);
    void countExpiredLinks(std::string name, std::ostream & output);
    void refreshLinks(std::string name);
  private:
    std::unordered_map< std::string, std::shared_ptr< Note > > notes_;
  };

  void cmdNote(std::istream & input, std::ostream & output, NoteBook & notebook);
  void cmdLine(std::istream & input, std::ostream & output, NoteBook & notebook);
  void cmdShow(std::istream & input, std::ostream & output, NoteBook & notebook);
  void cmdDrop(std::istream & input, std::ostream & output, NoteBook & notebook);
  void cmdLink(std::istream & input, std::ostream & output, NoteBook & notebook);
  void cmdMind(std::istream & input, std::ostream & output, NoteBook & notebook);
  void cmdHalt(std::istream & input, std::ostream & output, NoteBook & notebook);
  void cmdExpired(std::istream & input, std::ostream & output, NoteBook & notebook);
  void cmdRefresh(std::istream & input, std::ostream & output, NoteBook & notebook);
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
  cmds["mind"] = cmdMind;
  cmds["halt"] = cmdHalt;
  cmds["expired"] = cmdExpired;
  cmds["refresh"] = cmdRefresh;

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
  auto target = new_link.lock();
  if (!target) {
    throw std::logic_error("target note expired");
  }
  for (auto link : links_) {
    auto sp = link.lock();
    if (sp && sp->name_ == target->name_) {
      throw std::logic_error("duplicate link");
    }
  }
  links_.push_back(std::move(new_link));
}

void chernov::Note::removeLink(std::string link_name)
{
  for (auto it = links_.begin(); it != links_.end(); ++it) {
    auto sp = it->lock();
    if (sp && sp->name_ == link_name) {
      links_.erase(it);
      return;
    }
  }
  throw std::logic_error("link not found");
}

std::vector< std::string > chernov::Note::getValidLinkNames() const
{
  std::vector< std::string > valid_link_names;
  for (auto link : links_) {
    if (!link.expired()) {
      valid_link_names.push_back(link.lock()->name_);
    }
  }
  return valid_link_names;
}

size_t chernov::Note::countExpired() const noexcept
{
  size_t count = 0;
  for (auto link : links_) {
    count += link.expired();
  }
  return count;
}

void chernov::Note::cleanExpired()
{
  size_t size = links_.size();
  for (size_t i = 0; i < size; ++i) {
    if (links_[size - i - 1].expired()) {
      links_.erase(links_.begin() + (size - i - 1));
    }
  }
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

void chernov::NoteBook::mindNote(std::string name, std::ostream & output)
{
  std::vector< std::string > valid_link_names;
  try {
    valid_link_names = notes_.at(name)->getValidLinkNames();
  } catch (const std::out_of_range & e) {
    throw std::logic_error("note not found");
  }

  for (auto link_name : valid_link_names) {
    output << link_name << "\n";
  }
}

void chernov::NoteBook::haltLink(std::string name_from, std::string name_to)
{
  if (!notes_.count(name_to)) {
    throw std::logic_error("note-to not found");
  }

  try {
    notes_.at(name_from)->removeLink(name_to);
  } catch (const std::out_of_range & e) {
    throw std::logic_error("note-from not found");
  }
}

void chernov::NoteBook::countExpiredLinks(std::string name, std::ostream & output)
{
  try {
    output << notes_.at(name)->countExpired() << "\n";
  } catch (const std::out_of_range & e) {
    throw std::logic_error("note not found");
  }
}

void chernov::NoteBook::refreshLinks(std::string name)
{
  try {
    notes_.at(name)->cleanExpired();
  } catch (const std::out_of_range & e) {
    throw std::logic_error("note not found");
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
  input >> name >> std::quoted(line);;
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
  input >> name_from >> name_to;
  notebook.linkNotes(name_from, name_to);
}

void chernov::cmdMind(std::istream & input, std::ostream & output, NoteBook & notebook)
{
  std::string name;
  input >> name;
  notebook.mindNote(name, output);
}

void chernov::cmdHalt(std::istream & input, std::ostream &, NoteBook & notebook)
{
  std::string name_from, name_to;
  input >> name_from >> name_to;
  notebook.haltLink(name_from, name_to);
}

void chernov::cmdExpired(std::istream & input, std::ostream & output, NoteBook & notebook)
{
  std::string name;
  input >> name;
  notebook.countExpiredLinks(name, output);
}

void chernov::cmdRefresh(std::istream & input, std::ostream &, NoteBook & notebook)
{
  std::string name;
  input >> name;
  notebook.refreshLinks(name);
}
