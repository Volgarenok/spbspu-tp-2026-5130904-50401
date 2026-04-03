#include <iostream>
#include "console_notebook.hpp"

int main()
{
  std::string cmd;
  goltsov::Book all_notes;
  while (std::cin >> cmd)
  {
    std::string note_name;
    if (!(std::cin >> note_name))
    {
      std::cout << "<INVALID COMMAND>\n";
      continue;
    }
    try
    {
      all_notes.doAction(cmd, note_name);
    }
    catch (const std::exception& e)
    {
      std::cout << e.what();
    }
  }
}

