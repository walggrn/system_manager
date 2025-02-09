#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "commands/ICommand.h"
#include "commands/Read.h"
#include "messages/Exceptions.h"
#include "messages/Guides.h"
#include "utils/SystemManager.h"

using namespace std;

Read::Read() {
  name = "read";
  guide = READ_GUIDE;

  args.push_back(ArgData("PATH", nullopt));

  keys["--detail"] = KeyData(BOOLEAN, false);
  set_aliases("--detail", {"-dt"});
};

void Read::set_parametrs() {
  try {
    Path = get_arg_value(0);
    Detail = get<bool>(get_key_value("--detail"));
  } catch (const runtime_error &e) {
    throw_error(COMMAND_INCORRECT_PARAMETRS_EXCEPTION, e.what());
  }
};

void Read::command_execution() const {
  try {
    SystemManager manager;
    string data = manager.read(Path, Detail);
    cout << data << endl;
  } catch (const exception &e) {
    write_error(COMMAND_EXECUTION_EXCEPTION, e.what());
  }
};