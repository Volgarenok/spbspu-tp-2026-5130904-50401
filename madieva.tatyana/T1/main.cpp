#include<iostream>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include <iomanip>
struct Note {
  std::string name;
  std::vector< std::string > text;
  std::vector< std::weak_ptr< Note >> pointers;
};

auto  find_name(std::istream & in,
  std::ostream & out,
  std::map<std::string, std::shared_ptr< Note >> & notebook,
  std::string & name)
{
  if (!(in >> name)) {
    out << "<INVALID COMMAND>\n";
    return notebook.end();
  }
  auto it = notebook.find(name);
  if (it == notebook.end()) {
    out << "<INVALID COMMAND>\n";
    auto toignor = std::numeric_limits< std::streamsize >::max();
    in.ignore(toignor, '\n');
    return notebook.end();
  }
  return it;
}

bool the_end(std::istream & in,
  std::ostream & out)
{
  std::string remainder;
  getline(in, remainder);
  if (!remainder.empty()) {
    out << "<INVALID COMMAND>\n";
    return false;
  }
  return true;
}

void create_note(std::istream & in,
  std::ostream & out,
  std::map<std::string, std::shared_ptr< Note >> & notebook)
{
  std::string name;
  if (!(in >> name)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (!the_end(in, out)) {
    return;
  }
  if (notebook.count(name) == 0) {
    auto n = std::make_shared< Note >();
    n->name = name;
    notebook.insert({name, n});
  }
}

void add_line(std::istream & in,
  std::ostream & out,
  std::map<std::string, std::shared_ptr< Note >> & notebook)
{
  std::string name;
  auto it = find_name(in, out, notebook, name);
  if (it == notebook.end()) {
    return;
  }
  std::string str;
  if (!(in >> std::quoted(str))) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (!the_end(in, out)) {
    return;
  }
  std::shared_ptr< Note > note = it->second;
  note->text.push_back(str);
}

void show(std::istream & in,
  std::ostream & out,
  std::map<std::string, std::shared_ptr< Note >> & notebook)
{
  std::string name;
  auto it = find_name(in, out, notebook, name);
  if (it == notebook.end()) {
    return;
  }
  if (!the_end(in, out)) {
    return;
  }
  auto note = it->second;
  for (size_t i = 0; i < note->text.size(); ++i) {
    out << note->text[i] << "\n";
  }
}

void drop(std::istream & in,
  std::ostream & out,
  std::map<std::string, std::shared_ptr< Note >> & notebook)
{
  std::string name;
  auto it = find_name(in, out, notebook, name);
  if (it == notebook.end()) {
    return;
  }
  notebook.erase(it);
}


int main()
{
  std::map<std::string, std::shared_ptr< Note >> notebook;
  using cmd_t = void(*)(std::istream & in,
    std::ostream & out,
    std::map<std::string, std::shared_ptr< Note >> & notebook);
  std::map< std::string, cmd_t > cmds;
  cmds["note"] = create_note;
  cmds["line"] = add_line;
  cmds["show"] = show;
  cmds["drop"] = drop;

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.at(cmd)(std::cin, std::cout, notebook);
    } catch (const std::out_of_range &) {
      std::cout << "<INVALID COMMAND>\n";
      auto toignor = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignor, '\n');
    }
  }
  if (std::cin.eof()) {
    return 0;
  } else {
    std::cerr << "bad input\n";
  }
}

