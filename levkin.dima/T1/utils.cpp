#include "utils.hpp"

#include <iomanip>
#include <iostream>
#include <string>
namespace levkin {
std::string getWord(std::istream& in)
{
    std::string s;
    if (!(in >> s)) {
        if (in.eof()) {
            return "";
        }
        in.clear();
        return "";
    }

    return s;
}

std::string getQuote(std::istream& in)
{
    std::string s;
    if (!(in >> std::quoted(s))) {
        if (in.eof()) {
            return "";
        }
        in.clear();
        return "";
    }

    return s;
}

}
