#include <iostream>
#include <vector>
struct Note {
  std::string name_;
  std::string text_;
  std::vector<std::weak_ptr<std::string>> links_;
};
class NoteBook {
  public:
    NoteBook();
    ~NoteBook();

    void note(std::string& name);
    void line(std::string& text);
    void show(std::string& name);
    void drop(std::string& name);

    void link(std::string)

  private:


}
