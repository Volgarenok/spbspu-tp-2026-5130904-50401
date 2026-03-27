
#include "notebook.hpp"
namespace zubarev
{

  void zubarev::NoteBook::note(const std::string& name)
  {
    if (notes_.find(name) == notes_.end()) {
      notes_[name] = std::make_shared< Note >(name);
    }
  }
  void zubarev::NoteBook::line(const std::string& name, std::string& text)
  {
    if (notes_.find(name) != notes_.end()) {
      notes_[name]->lines_.push_back(text);
    }
  }
  void zubarev::NoteBook::show(const std::string& name, std::ostream& out) const
  {
    auto it = notes_.find(name);
    if (it != notes_.end()) {
      auto text = it->second->lines_;
      for (size_t i = 0; i < text.size(); ++i) {
        out << text[i] << '\n';
      }
    }
  }
  void zubarev::NoteBook::drop(const std::string& name)
  {
    if (notes_.find(name) != notes_.end()) {
      notes_.erase(name);
    }
  }

  void zubarev::NoteBook::link(const std::string& noteFrom, const std::string& noteTo)
  {
    if (notes_.find(noteFrom) != notes_.end() && notes_.find(noteTo) != notes_.end()) {
      notes_[noteFrom]->links_.push_back(notes_[noteTo]);
    }
  }
  void zubarev::NoteBook::halt(const std::string& noteFrom, const std::string& noteTo)
  {
    if (notes_.find(noteFrom) != notes_.end() && notes_.find(noteTo) != notes_.end()) {
      auto& linksFrom = notes_[noteFrom]->links_;
      for (auto it = linksFrom.begin(); it != linksFrom.end(); ++it) {
        if (it->lock()->name_ == noteTo) {
          linksFrom.erase(it);
          break;
        }
      }
    }
  }
  void zubarev::NoteBook::mind(const std::string& noteFrom, std::ostream& out) const
  {
    auto it = notes_.find(noteFrom);
    if (it != notes_.end()) {
      const auto& linksFrom = it->second->links_;
      for (auto itLink = linksFrom.begin(); itLink != linksFrom.end(); ++itLink) {
        if (itLink->lock()) {
          out << itLink->lock()->name_ << '\n';
        }
      }
    }
  }
  void zubarev::NoteBook::expired(const std::string& noteFrom, std::ostream& out) const
  {
    auto it = notes_.find(noteFrom);
    size_t count = 0;
    if (it != notes_.end()) {

      const auto& linksFrom = it->second->links_;
      for (auto itLink = linksFrom.begin(); itLink != linksFrom.end(); ++itLink) {
        if (!itLink->lock()) {
          count++;
        }
      }
    }
    out << count << '\n';
  }
  void zubarev::NoteBook::refresh(const std::string& noteFrom)
  {
    auto it = notes_.find(noteFrom);
    if (it != notes_.end()) {

      auto& linksFrom = it->second->links_;
      for (auto itLink = linksFrom.begin(); itLink != linksFrom.end(); ++itLink) {
        if (!itLink->lock()) {
          linksFrom.erase(itLink);
        }
      }
    }
  }
}
