#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "commands/Dir.h"
#include "commands/ICommand.h"
#include "messages/Exceptions.h"
#include "messages/Guides.h"
#include "utils/SystemManager.h"

using namespace std;

Dir::Dir() {
  name = "dir";
  guide = DIR_GUIDE;

  args.push_back(ArgData("PATH", "."));

  keys["--recursive"] = KeyData(BOOLEAN, false);
  keys["--depth"] = KeyData(INTEGER, -1, {"--recursive"});
  set_aliases("--recursive", {"-r"});
  set_aliases("--depth", {"-d"});
};

void Dir::set_parametrs() {
  try {
    Path = get_arg_value(0);
    Recursive = get<bool>(get_key_value("--recursive"));
    Depth = get<int>(get_key_value("--depth"));
  } catch (const runtime_error &e) {
    throw_error(COMMAND_INCORRECT_PARAMETRS_EXCEPTION, e.what());
  }
};

void Dir::command_execution() const {
  try {
    SystemManager manager;
    manager.list(Path, Recursive, Depth);
  } catch (const exception &e) {
    write_error(COMMAND_EXECUTION_EXCEPTION, e.what());
  }
};