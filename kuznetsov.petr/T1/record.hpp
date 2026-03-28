#ifndef RECORD_HPP
#define RECORD_HPP
namespace kuznetsov {

  struct Record {
    std::string name;
    std::vector< std::string > lines;
    std::vector< std::weak_ptr > refs;
  }
}
#endif

