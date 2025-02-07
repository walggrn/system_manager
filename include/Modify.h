#ifndef RENAME_H
#define RENAME_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Guides.h"
#include "ICommand.h"
#include "SystemManager.h"

using namespace std;

class Modify : public ICommand {
private:
  string CurrentPath;
  string NewPath;
  bool Recreate;

public:
  Modify() {
    name = "modify";
    guide = MODIFY_GUIDE;

    args.push_back(ArgData("PATH1", nullopt));
    args.push_back(ArgData("PATH2", nullopt));

    keys["--recreate"] = KeyData(BOOLEAN, false);
    set_aliases("--recreate", {"-rc"});
  }

  void set_parametrs() override {
    try {
      CurrentPath = get_arg_value(0);
      NewPath = get_arg_value(1);
      Recreate = get<bool>(get_key_value("--recreate"));
    } catch (const runtime_error &e) {
      throw_error(COMMAND_INCORRECT_PARAMETRS_EXCEPTION, e.what());
    }
  }

  void command_execution() const override {
    try {
      SystemManager manager;
      manager.modify(CurrentPath, NewPath, Recreate);
    } catch (const exception &e) {
      write_error(COMMAND_EXECUTION_EXCEPTION, e.what());
    }
  }
};

#endif