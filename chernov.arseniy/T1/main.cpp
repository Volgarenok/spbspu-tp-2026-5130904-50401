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
  private:
    std::string name_;
    std::string text_;
    std::vector< std::weak_ptr< Note > > links_;
  };

  class NoteBook {
  public:
    void createNote(std::string name);
  private:
    std::unordered_map< std::string, std::shared_ptr< Note > > notes_;
  };

  void cmdNote(std::istream & input, std::ostream & output, NoteBook & notebook);
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

void chernov::NoteBook::createNote(std::string name)
{
  notes_[name] = std::make_shared< Note >(name);
}

void chernov::cmdNote(std::istream & input, std::ostream &, NoteBook & notebook)
{
  std::string name;
  input >> name;
  notebook.createNote(name);
}
