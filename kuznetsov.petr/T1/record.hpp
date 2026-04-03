#ifndef RECORD_HPP
#define RECORD_HPP
#include <string>
#include <vector>
#include <memory>
namespace kuznetsov {
  struct Record {
    Record(std::string name):
      name_(name),
      lines_{},
      refs_{}
    {}

    std::string name_;
    std::vector< std::string > lines_;
    std::vector< std::weak_ptr< Record > > refs_;
  };
}
#endif

