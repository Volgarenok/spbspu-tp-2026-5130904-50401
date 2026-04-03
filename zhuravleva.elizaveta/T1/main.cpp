#include "note.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include <unordered_map>
#include <limits>

void note(std::istream& in, std::ostream&, zhuravleva::Db& db)
{
  std::string name;
  in >> name;
  zhuravleva::create_note(db, name);
}
void line(std::istream& in, std::ostream&, zhuravleva::Db& db)
{
  std::string name, text;
  in>> name >> std::quoted(text);
  zhuravleva::add_line(db, name, text);
}
void show(std::istream& in, std::ostream& out, zhuravleva::Db& db)
{
  std::string name;
  in >> name;
  auto lines = zhuravleva::show(db, name);
  for (size_t i = 0; i < lines.size();i++)
  {
    out << lines[i] << "\n";
  }
}
void drop(std::istream& in, std::ostream&, zhuravleva::Db& db)
{
  std::string name;
  in >> name;
  zhuravleva::drop(db,name);
}
void link(std::istream& in, std::ostream&, zhuravleva::Db& db)
{
  std::string from, to;
  in >> from >> to;
  zhuravleva::link(db, from, to);
}
void halt(std::istream& in, std::ostream&, zhuravleva::Db& db)
{
  std::string from,to;
  in >> from >> to;
  zhuravleva::halt(db, from, to);
}
void mind(std::istream& in, std::ostream& out, zhuravleva::Db& db)
{
  std::string name;
  in >> name;
  auto res = zhuravleva::mind(db, name);
  for (size_t i = 0; i < res.size(); ++i)
  {
    out << res[i] << "\n";
  }
}
void expired(std::istream& in, std::ostream& out, zhuravleva::Db& db)
{
  std::string name;
  in >> name;
  out << zhuravleva::expired(db, name);
}
void refresh(std::istream& in, std::ostream&, zhuravleva::Db& db)
{
  std::string name;
  in >> name;
  zhuravleva::refresh(db,name);
}

int main()
{
  zhuravleva::Db db;
  using cmd_t = void(*)(std::istream&, std::ostream&, zhuravleva::Db&);
  std::unordered_map<std::string, cmd_t> cmds;
  cmds["note"] = note;
  cmds["line"] = line;
  cmds["show"] = show;
  cmds["drop"] = drop;
  cmds["link"] = link;
  cmds["halt"] = halt;
  cmds["mind"] = mind;
  cmds["expired"] = expired;
  cmds["refresh"] = refresh;

  std::string cmd;

  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin,std::cout,db);
    }
    catch (const std::out_of_range&)
    {
      std::cout << "INVALID COMMAND\n";
      auto toignore = std::numeric_limits<std::streamsize>::max();
      std::cin.ignore(toignore, '\n');
    }
    catch (const std::logic_error & e)
    {
      std::cout << "< INVALID COMMAND " << e.what() << ">\n ";
    }
  }
  if (!std::cin.eof())
  {
    std::cerr << "bad input \n";
    return 1;
  }
  return 0;
}
