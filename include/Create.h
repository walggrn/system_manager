#ifndef CREATE_H
#define CREATE_H

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "Exceptions.h"
#include "Guides.h"
#include "ICommand.h"
#include "SystemManager.h"

using namespace std;

class Create : public ICommand {
private:
  string Path;
  bool Recreate;
  bool Directory;

public:
  Create() {
    name = "create";
    guide = CREATE_GUIDE;

    args.push_back(ArgData("PATH", nullopt));

    keys["--recreate"] = KeyData(BOOLEAN, false);
    keys["--directory"] = KeyData(BOOLEAN, false);
    set_aliases("--recreate", {"-rc"});
    set_aliases("--directory", {"-d"});
  }

  void set_parametrs() override {
    try {
      Path = get_arg_value(0);
      Recreate = get<bool>(get_key_value("--recreate"));
      Directory = get<bool>(get_key_value("--directory"));
    } catch (const runtime_error &e) {
      throw_error(COMMAND_INCORRECT_PARAMETRS_EXCEPTION, e.what());
    }
  }

  void command_execution() const override {
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
  }
};

#endif