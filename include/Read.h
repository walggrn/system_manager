#ifndef READ_H
#define READ_H

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

class Read : public ICommand {
private:
  string Path;
  bool Detail;

public:
  Read() {
    name = "read";
    guide = READ_GUIDE;

    args.push_back(ArgData("PATH", nullopt));

    keys["--detail"] = KeyData(BOOLEAN, false);
    set_aliases("--detail", {"-dt"});
  }

  void set_parametrs() override {
    try {
      Path = get_arg_value(0);
      Detail = get<bool>(get_key_value("--detail"));
    } catch (const runtime_error &e) {
      throw_error(COMMAND_INCORRECT_PARAMETRS_EXCEPTION, e.what());
    }
  }

  void command_execution() const override {
    try {
      SystemManager manager;
      string data = manager.read(Path, Detail);
      cout << data << endl;
    } catch (const exception &e) {
      write_error(COMMAND_EXECUTION_EXCEPTION, e.what());
    }
  }
};

#endif