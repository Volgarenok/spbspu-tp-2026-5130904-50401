#include "notebook.hpp"

zharov::Book::Book()
{
  cmds_["note"] = note;
  cmds_["show"] = show;
  cmds_["drop"] = drop;
  cmds_["mind"] = mind;
  cmds_["expired"] = expired;
  cmds_["refresh"] = refresh;
  cmds_["link"] = link;
  cmds_["halt"] = halt;
  cmds_["line"] = line;
}