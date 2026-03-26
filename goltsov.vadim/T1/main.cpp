#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <memory>
#include <unordered_map>


namespace goltsov
{
  void note (std::shared_ptr< Note >&);
  void show (std::shared_ptr< Note >&);
  void drop (std::shared_ptr< Note >&);
  void mind (std::shared_ptr< Note >&);
  void expired (std::shared_ptr< Note >&);
  void refresh (std::shared_ptr< Note >&);
  void link (std::shared_ptr< Note >&, std::shared_ptr< Note >&);
  void halt (std::shared_ptr< Note >&, std::shared_ptr< Note >&);
  void line (std::shared_ptr< Note >&, std::string&);

  struct Note
  {
    std::string text;
    std::vector< std::weak_ptr< Note > > links;
  };

  struct Book
  {
    Book();
    std::unordered_map< std::string, std::shared_ptr< Note > > all_notes;

    std::unordered_map< std::string, void(*)(std::shared_ptr< Note >&) > funcs_only_link;
    std::unordered_map< std::string, void(*)(std::shared_ptr< Note >&, std::shared_ptr< Note >&) > funcs_two_link;
    std::unordered_map< std::string, void(*)(std::shared_ptr< Note >&, std::string&) > funcs_link_with_text;

    void doAction(std::string&, std::string& note_name, std::istream&);
  };

  Book::Book()
  {
    funcs_only_link["note"] = note;
    funcs_only_link["show"] = show;
    funcs_only_link["drop"] = drop;
    funcs_only_link["mind"] = mind;
    funcs_only_link["expired"] = expired;
    funcs_only_link["refresh"] = refresh;
    funcs_two_link["link"] = link;
    funcs_two_link["note"] = halt;
    funcs_link_with_text["line"] = line;
  }

  void Book::doAction(std::string& name_func, std::string& note_name, std::istream& in)
  {
    if (name_func == "line")
    {
      std::string quoted_text;
      in >> std::quoted(quoted_text);
      funcs_link_with_text.at(name_func)(all_notes.at(note_name), quoted_text);
    }
    else if (name_func == "link" || name_func == "halt")
    {
      std::string note_to;
      in >> note_to;
      funcs_two_link.at(name_func)(all_notes.at(note_name), all_notes.at(note_to));
    }
    else
    {
      try
      {
        funcs_only_link.at(name_func)(all_notes.at(note_name));
      }
      catch(const std::exception& e)
      {
        std::cerr << "<INVALID COMMAND>\n";
      }
    }
  }
}

int main()
{
  
}
