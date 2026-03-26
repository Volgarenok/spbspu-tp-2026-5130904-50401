#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <memory>
#include <unordered_map>

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

    void note (std::string&);
    void show (std::string&);
    void drop (std::string&);
    void mind (std::string&);
    void expired (std::string&);
    void refresh (std::string&);
    void link (std::string&, std::string&);
    void halt (std::string&, std::string&);
    void line (std::string&, std::string&);
  };

  Book::Book()
  {
    funcs_only_word["note"] = &Book::note;
    funcs_only_word["show"] = &Book::show;
    funcs_only_word["drop"] = &Book::drop;
    funcs_only_word["mind"] = &Book::mind;
    funcs_only_word["expired"] = &Book::expired;
    funcs_only_word["refresh"] = &Book::refresh;
    funcs_two_link["link"] = &Book::link;
    funcs_two_link["halt"] = &Book::halt;
    funcs_two_words["line"] = &Book::line;
  }

  void Book::doAction(std::string& name_func, std::string& note_name)
  {
    if (name_func == "line")
    {
      std::string quoted_text;
      std::cin >> std::quoted(quoted_text);
      (this->*funcs_two_words.at(name_func))(note_name, quoted_text);
    }
    else if (name_func == "link" || name_func == "halt")
    {
      std::string note_to;
      std::cin >> note_to;
      (this->*funcs_two_link.at(name_func))(note_name, note_to);
    }
    else
    {
      try
      {
        (this->*funcs_only_word.at(name_func))(note_name);
      }
      catch(const std::exception& e)
      {
        std::cerr << "<INVALID COMMAND>\n";
      }
    }
  }

  void Book::note (std::string& note_name)
  {
    for (size_t i = 0; i < all_notes.size(); ++i)
    {
      if (all_notes[i]->name == note_name)
      {
        return;
      }
    }
    std::shared_ptr< Note > new_note = std::shared_ptr< Note >(new Note);
    new_note->name = note_name;
    new_note->text = "";
    all_notes.push_back(new_note);
  }
  void Book::show (std::string& note_name)
  {
    for (size_t i = 0; i < all_notes.size(); ++i)
    {
      if (all_notes[i]->name == note_name)
      {
        std::cout << all_notes[i]->text;
        return;
      }
    }
    throw std::logic_error("<INVALID COMMAND>\n");
  }
  void Book::drop (std::string& note_name)
  {
    for (size_t i = 0; i < all_notes.size(); ++i)
    {
      if (all_notes[i]->name == note_name)
      {
        all_notes.erase(all_notes.begin() + i);
        return;
      }
    }
    throw std::logic_error("<INVALID COMMAND>\n");
  }
  void Book::mind (std::string& note_name)
  {
    std::shared_ptr< Note > temp = nullptr;
    for (size_t i = 0; i < all_notes.size(); ++i)
    {
      if (all_notes[i]->name == note_name)
      {
        temp = all_notes[i];
        break;
      }
    }
    if (temp == nullptr)
    {
      throw std::logic_error("<INVALID COMMAND>\n");
    }
    for (size_t i = 0; i < temp->links.size(); ++i)
    {
      if (!temp->links[i].expired())
      {
        std::cout << temp->links[i].lock()->name << '\n';
      }
    }
  }
  void Book::expired (std::string& note_name)
  {
    std::shared_ptr< Note > temp = nullptr;
    for (size_t i = 0; i < all_notes.size(); ++i)
    {
      if (all_notes[i]->name == note_name)
      {
        temp = all_notes[i];
        break;
      }
    }
    if (temp == nullptr)
    {
      throw std::logic_error("<INVALID COMMAND>\n");
    }
    size_t ans = 0;
    for (size_t i = 0; i < temp->links.size(); ++i)
    {
      if (temp->links[i].expired())
      {
        ans++;
      }
    }
    std::cout << ans << '\n';
  }
  void Book::refresh (std::string& note_name)
  {
    std::shared_ptr< Note > temp = nullptr;
    for (size_t i = 0; i < all_notes.size(); ++i)
    {
      if (all_notes[i]->name == note_name)
      {
        temp = all_notes[i];
        break;
      }
    }
    if (temp == nullptr)
    {
      throw std::logic_error("<INVALID COMMAND>\n");
    }
    for (size_t i = 0; i < temp->links.size(); ++i)
    {
      if (temp->links[i].expired())
      {
        temp->links.erase(temp->links.begin() + i);
        --i;
      }
    }
  }
  void Book::link (std::string& note_from, std::string& note_to)
  {
    std::shared_ptr< Note > from = nullptr;
    std::shared_ptr< Note > to = nullptr;
    for (size_t i = 0; i < all_notes.size(); ++i)
    {
      if (all_notes[i]->name == note_from)
      {
        from = all_notes[i];
      }
      if (all_notes[i]->name == note_to)
      {
        to = all_notes[i];
      }
    }
    if (from == nullptr || to == nullptr)
    {
      throw std::logic_error("<INVALID COMMAND>\n");
    }
    for (size_t i = 0; i < from->links.size(); ++i)
    {
      if (from->links[i].lock() == to)
      {
        return;
      }
    }
    from->links.push_back(to);
  }
  void Book::halt (std::string& note_from, std::string& note_to)
  {
    std::shared_ptr< Note > from = nullptr;
    std::shared_ptr< Note > to = nullptr;
    for (size_t i = 0; i < all_notes.size(); ++i)
    {
      if (all_notes[i]->name == note_from)
      {
        from = all_notes[i];
      }
      if (all_notes[i]->name == note_to)
      {
        to = all_notes[i];
      }
    }
    if (from == nullptr || to == nullptr)
    {
      throw std::logic_error("<INVALID COMMAND>\n");
    }
    for (size_t i = 0; i < from->links.size(); ++i)
    {
      if (from->links[i].lock() == to)
      {
        from->links.erase(from->links.begin() + i);
        return;
      }
    }
  }
  void Book::line (std::string& note_name, std::string& quoted_text)
  {
    for (size_t i = 0; i < all_notes.size(); ++i)
    {
      if (all_notes[i]->name == note_name)
      {
        all_notes[i]->text.append(quoted_text + '\n');
        return;
      }
    }
    note(note_name);
    all_notes.back()->text.append(quoted_text + '\n');
  }
}

int main()
{
  std::string cmd;
  goltsov::Book all_notes;
  while (std::cin >> cmd)
  {
    std::string note_name;
    if (!(std::cin >> note_name))
    {
      std::cout << "<INVALID COMMAND>\n";
      continue;
    }
    try
    {
      all_notes.doAction(cmd, note_name);
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what();
    }
  }
}
