#ifndef NOTE_HPP
#define NOTE_HPP
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace vasyakin
{
  class Note
  {
  public:
    explicit Note(const std::string& name);
    void noteLine(const std::string& txt);
    void noteShow() const;
    void noteLink(const std::weak_ptr< Note >& link);
    void noteHalt(const std::shared_ptr< Note >& target);
    void noteMind() const;
    size_t noteExpired() const;
    void noteRefresh();
    const std::string& getName() const;
  private:
    std::string name_;
    std::vector< std::string > lines_;
    std::vector< std::weak_ptr< Note > > links_;
  };

  using NotesMap = std::unordered_map<std::string, std::shared_ptr< Note > >;
}

#endif
