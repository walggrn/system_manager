#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "commands/Copy.h"
#include "commands/ICommand.h"
#include "messages/Exceptions.h"
#include "messages/Guides.h"
#include "utils/SystemManager.h"

using namespace std;

Copy::Copy() {
  name = "copy";
  guide = COPY_GUIDE;

  args.push_back(ArgData("PATH1", nullopt));
  args.push_back(ArgData("PATH2", nullopt));

  keys["--t"] = KeyData(BOOLEAN, false);
  keys["--recreate"] = KeyData(BOOLEAN, false);
  keys["--directory"] = KeyData(BOOLEAN, false);
  keys["--recursive"] = KeyData(BOOLEAN, false, {"--directory"});
  set_aliases("--recreate", {"-rc"});
  set_aliases("--directory", {"-d"});
  set_aliases("--recursive", {"-r"});
};

void Copy::set_parametrs() {
  try {
    PathFrom = get_arg_value(0);
    PathTo = get_arg_value(1);
    Recreate = get<bool>(get_key_value("--recreate"));
    Directory = get<bool>(get_key_value("--directory"));
    Recursive = get<bool>(get_key_value("--recursive"));
  } catch (const runtime_error &e) {
    throw_error(COMMAND_INCORRECT_PARAMETRS_EXCEPTION, e.what());
  }
}

void Copy::command_execution() const {
  try {
    SystemManager manager;
    if (Directory) {
      manager.copy_directory(PathFrom, PathTo, Recreate, Recursive);
    } else {
      manager.copy_file(PathFrom, PathTo, Recreate);
    }
    cout << "Done." << endl;
  } catch (const runtime_error &e) {
    write_error(COMMAND_EXECUTION_EXCEPTION, e.what());
  }
};