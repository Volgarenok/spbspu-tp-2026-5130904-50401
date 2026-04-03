#ifndef NOTE_HPP
#define NOTE_HPP

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace burukov
{
  class Note
  {
  public:
    explicit Note(const std::string& name);

    void addLine(const std::string& text);
    void showLines() const;
    void addLink(const std::weak_ptr< Note >& link);
    void removeLink(const std::shared_ptr< Note >& target);
    void showLinks() const;
    size_t countExpired() const;
    void clearExpired();

    const std::string& getName() const;

  private:
    std::string name_;
    std::vector< std::string > lines_;
    std::vector< std::weak_ptr< Note > > links_;
  };

  using NotesMap = std::unordered_map< std::string, std::shared_ptr< Note > >;
}

#endif