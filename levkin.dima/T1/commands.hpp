#ifndef COMMANDS
#define COMMANDS

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>



namespace levkin {
  class Note;

  using Database = std::unordered_map< std::string, std::shared_ptr< Note > >;
  using Link = std::weak_ptr< Note >;
  using cmd_t = void (*)(std::istream&, std::ostream&, Database&);
  using Cmds = std::unordered_map< std::string, cmd_t >;



  class Note
  {
  public:
    Note(std::string s);
    void addContent(std::string c);
    const std::vector< std::string >& getContent() const;

    const std::vector< Link >& getLinks() const;
    void rmLink(Link ptr);
    void addLink(Link ptr);
    void cleanExpired();
    const std::string& getId() const;

  private:
    std::string id;
    std::vector< std::string > content;
    std::vector< Link > links;

    template < typename F > void eraseIf(F f)
    {
      size_t writeIndex = 0;
      for (size_t readIndex = 0; readIndex < links.size(); ++readIndex) {
        if (!f(links[readIndex])) {
          links[writeIndex] = links[readIndex];
          writeIndex++;
        }
      }
      links.resize(writeIndex);
    }
  };

  void note(std::istream& in, std::ostream& out, Database& db);
  void line(std::istream& in, std::ostream& out, Database& db);
  void show(std::istream& in, std::ostream& out, Database& db);
  void drop(std::istream& in, std::ostream& out, Database& db);
  void link(std::istream& in, std::ostream& out, Database& db);
  void halt(std::istream& in, std::ostream& out, Database& db);
  void mind(std::istream& in, std::ostream& out, Database& db);
  void expired(std::istream& in, std::ostream& out, Database& db);
  void refresh(std::istream& in, std::ostream& out, Database& db);

  Cmds getCmds();
}

#endif
