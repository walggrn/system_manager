#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "commands/Create.h"
#include "commands/ICommand.h"
#include "messages/Exceptions.h"
#include "messages/Guides.h"
#include "utils/SystemManager.h"

using namespace std;

Create::Create() {
  name = "create";
  guide = CREATE_GUIDE;

  args.push_back(ArgData("PATH", nullopt));

  keys["--recreate"] = KeyData(BOOLEAN, false);
  keys["--directory"] = KeyData(BOOLEAN, false);
  set_aliases("--recreate", {"-rc"});
  set_aliases("--directory", {"-d"});
};

void Create::set_parametrs() {
  try {
    Path = get_arg_value(0);
    Recreate = get<bool>(get_key_value("--recreate"));
    Directory = get<bool>(get_key_value("--directory"));
  } catch (const runtime_error &e) {
    throw_error(COMMAND_INCORRECT_PARAMETRS_EXCEPTION, e.what());
  }
};

void Create::command_execution() const {
  try {
    SystemManager manager;
    if (Directory) {
      manager.create_directory(Path, Recreate);
    } else {
      manager.create_file(Path, Recreate);
    }
    cout << "Done." << endl;
  } catch (const runtime_error &e) {
    write_error(COMMAND_EXECUTION_EXCEPTION, e.what());
  }
};