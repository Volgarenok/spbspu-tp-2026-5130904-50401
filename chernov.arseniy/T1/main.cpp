#include <iostream>
#include <iomanip>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace chernov {
  class Note {
  public:
    std::string getName() const noexcept;
    std::string getText() const noexcept;
  private:
    std::string name_;
    std::string text_;
    std::vector< std::weak_ptr< Note > > links_;
  };
}

int main()
{
  std::istream & input = std::cin;
  std::string cmd;
  while (input >> cmd) {
  }
  if (!input.eof()) {
    std::cerr << "bad input\n";
    return 1;
  }
}

std::string chernov::Note::getName() const noexcept
{
  return name_;
}

std::string chernov::Note::getText() const noexcept
{
  return text_;
}
