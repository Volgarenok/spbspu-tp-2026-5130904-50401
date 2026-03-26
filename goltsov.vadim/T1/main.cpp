#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <memory>
#include <unordered_map>
#include <set>

namespace goltsov
{
  struct Note
  {
    std::string text;
    std::set< std::weak_ptr< Note > > links;
  };

  struct Book
  {
    Book();
    std::unordered_map< std::string, std::shared_ptr< Note > > all_notes;

    std::unordered_map< std::string, void(Book::*)(std::string&) > funcs_only_word;
    std::unordered_map< std::string, void(Book::*)(std::shared_ptr< Note >&) > funcs_only_link;
    std::unordered_map< std::string, void(Book::*)(std::shared_ptr< Note >&, std::shared_ptr< Note >&) > funcs_two_link;
    std::unordered_map< std::string, void(Book::*)(std::shared_ptr< Note >&, std::string&) > funcs_link_with_text;

    void doAction(std::string&, std::string& note_name, std::istream&);

    void note (std::string&);
    void show (std::shared_ptr< Note >&);
    void drop (std::string&);
    void mind (std::shared_ptr< Note >&);
    void expired (std::shared_ptr< Note >&);
    void refresh (std::shared_ptr< Note >&);
    void link (std::shared_ptr< Note >&, std::shared_ptr< Note >&);
    void halt (std::shared_ptr< Note >&, std::shared_ptr< Note >&);
    void line (std::shared_ptr< Note >&, std::string&);
  };

  Book::Book()
  {
    funcs_only_word["note"] = note;
    funcs_only_word["drop"] = drop;
    funcs_only_link["show"] = show;
    funcs_only_link["mind"] = mind;
    funcs_only_link["expired"] = expired;
    funcs_only_link["refresh"] = refresh;
    funcs_two_link["link"] = link;
    funcs_two_link["note"] = halt;
    funcs_link_with_text["line"] = line;
  }

  void Book::doAction(std::string& name_func, std::string& note_name, std::istream& in)
  {
    if (name_func == "note")
    {
      (this->*funcs_only_word.at(name_func))(note_name);
    }
    else if (name_func == "line")
    {
      std::string quoted_text;
      in >> std::quoted(quoted_text);
      (this->*funcs_link_with_text.at(name_func))(all_notes.at(note_name), quoted_text);
    }
    else if (name_func == "link" || name_func == "halt")
    {
      std::string note_to;
      in >> note_to;
      (this->*funcs_two_link.at(name_func))(all_notes.at(note_name), all_notes.at(note_to));
    }
    else
    {
      try
      {
        (this->*funcs_only_link.at(name_func))(all_notes.at(note_name));
      }
      catch(const std::exception& e)
      {
        std::cerr << "<INVALID COMMAND>\n";
      }
    }
  }

  void Book::note (std::string& note_name)
  {
    if (all_notes.find(note_name) == all_notes.end() || all_notes[note_name] == nullptr)
    {
      all_notes[note_name] = std::shared_ptr< Note > {};
    }
  }
  void Book::show (std::shared_ptr< Note >& note)
  {
    std::cout << note->text;
  }
  void Book::drop (std::string& note_name)
  {
    all_notes.erase(note_name);
  }
  void Book::mind (std::shared_ptr< Note >&)
  {}
  void Book::expired (std::shared_ptr< Note >&)
  {}
  void Book::refresh (std::shared_ptr< Note >&)
  {}
  void Book::link (std::shared_ptr< Note >& name_from, std::shared_ptr< Note >& name_to)
  {
    name_from->links.insert(name_to);
  }
  void Book::halt (std::shared_ptr< Note >&, std::shared_ptr< Note >&)
  {}
  void Book::line (std::shared_ptr< Note >&, std::string&)
  {}
}

int main()
{
  
}
