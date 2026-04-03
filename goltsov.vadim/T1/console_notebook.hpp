#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace goltsov
{
  struct Note
  {
    std::string name;
    std::string text;
    std::vector< std::weak_ptr< Note > > links;
  };

  struct Book
  {
    Book();
    std::vector< std::shared_ptr< Note > > all_notes;

    std::unordered_map< std::string, void(Book::*)(std::string&) > funcs_only_word;
    std::unordered_map< std::string, void(Book::*)(std::string&, std::string&) > funcs_two_link;
    std::unordered_map< std::string, void(Book::*)(std::string&, std::string&) > funcs_two_words;

    void doAction(std::string&, std::string&);

    void note(std::string&);
    void show(std::string&);
    void drop(std::string&);
    void mind(std::string&);
    void expired(std::string&);
    void refresh(std::string&);
    void link(std::string&, std::string&);
    void halt(std::string&, std::string&);
    void line(std::string&, std::string&);
  };
}

