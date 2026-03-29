#ifndef NOTE_HPP
#define NOTE_HPP
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace sedov
{
  struct Note
  {
    explicit Note(const std::string & str);
    void nLine(const std::string & l);
    void nShow() const;
    void nLink(const std::weak_ptr< Note > & l);
    void nHalt(const std::shared_ptr< Note > & t);
    void nMind() const;
    size_t nExpired() const;
    void nRefresh();
    const std::string& getName() const;
  private:
    std::string name_;
    std::vector< std::string > lines_;
    std::vector< std::weak_ptr< Note > > links_;
  };

  using NUM = std::unordered_map<std::string, std::shared_ptr< Note > >;
};

#endif
