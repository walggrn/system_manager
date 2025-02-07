#ifndef DIR_H
#define DIR_H

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "Exceptions.h"
#include "Guides.h"
#include "ICommand.h"
#include "SystemManager.h"

using namespace std;

class Dir : public ICommand {
private:
  string Path;
  bool Recursive;
  bool Depth;

public:
  Dir() {
    name = "dir";
    guide = DIR_GUIDE;

    args.push_back(ArgData("PATH", "."));

    keys["--recursive"] = KeyData(BOOLEAN, false);
    keys["--depth"] = KeyData(INTEGER, -1, {"--recursive"});
    set_aliases("--recursive", {"-r"});
    set_aliases("--depth", {"-d"});
  }

  void set_parametrs() override {
    try {
      Path = get_arg_value(0);
      Recursive = get<bool>(get_key_value("--recursive"));
      Depth = get<int>(get_key_value("--depth"));
    } catch (const runtime_error &e) {
      throw_error(COMMAND_INCORRECT_PARAMETRS_EXCEPTION, e.what());
    }
  }

  void command_execution() const override {
    try {
      SystemManager manager;
      manager.list(Path, Recursive, Depth);
    } catch (const exception &e) {
      write_error(COMMAND_EXECUTION_EXCEPTION, e.what());
    }
  }
};

#endif