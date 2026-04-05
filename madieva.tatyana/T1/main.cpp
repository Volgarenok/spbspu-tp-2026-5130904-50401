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
  if (!the_end(in, out)) {
    return;
  }
  notebook.erase(it);
}

void link(std::istream & in,
  std::ostream & out,
  std::map<std::string, std::shared_ptr< Note >> & notebook)
{
  std::string name1;
  auto it1 = find_name(in, out, notebook, name1);
  if (it1 == notebook.end()) {
    return;
  }
  std::string name2;
  auto it2 = find_name(in, out, notebook, name2);
  if (it2 == notebook.end()) {
    return;
  }
  if (it1 == it2) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (!the_end(in, out)) {
    return;
  }
  std::shared_ptr< Note > note = it1->second;
  std::weak_ptr< Note > obj = it2->second;
  bool repeat = false;
  for (size_t i = 0; i < note->pointers.size() && !repeat; ++i) {
    if (!note->pointers[i].expired() && note->pointers[i].lock() == it2->second) {
      repeat = true;
    }
  }
  note->pointers.push_back(obj);
}

void halt(std::istream & in,
  std::ostream & out,
  std::map<std::string, std::shared_ptr< Note >> & notebook)
{
  std::string name1;
  auto it1 = find_name(in, out, notebook, name1);
  if (it1 == notebook.end()) {
    return;
  }
  std::string name2;
  if (!(in >> name2)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (!the_end(in, out)) {
    return;
  }
  auto node = it1->second;
  auto it = node->pointers.begin();
  for (; it != node->pointers.end(); ++it) {
    auto temp = it->lock();
    if (temp && temp->name == name2) {
      node->pointers.erase(it);
      return;
    }
  }
}

void mind(std::istream & in,
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
  auto itp = it->second->pointers.begin();
  for (; itp != it->second->pointers.end(); ++itp) {
    if (itp->lock()) {
      out << itp->lock()->name << "\n";
    }
  }
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
  cmds["link"] = link;
  cmds["halt"] = halt;
  cmds["mind"] = mind;

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

