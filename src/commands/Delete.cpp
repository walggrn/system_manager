#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

#include "commands/Delete.h"
#include "commands/ICommand.h"
#include "messages/Exceptions.h"
#include "messages/Guides.h"
#include "utils/SystemManager.h"

using namespace std;

Delete::Delete() {
  name = "delete";
  guide = DELETE_GUIDE;

  args.push_back(ArgData("PATH", nullopt));

  keys["--directory"] =
      KeyData(BOOLEAN, false, {}, {"--output", "--line", "--skip", "--length"});
  keys["--output"] = KeyData(BOOLEAN, false, {"--line"}, {"--directory"});
  keys["--line"] = KeyData(INTEGER, -1, {}, {"--directory"});
  keys["--skip"] = KeyData(INTEGER, 0, {"--line"}, {"--directory"});
  keys["--length"] = KeyData(INTEGER, -1, {"--line"}, {"--directory"});
  set_aliases("--directory", {"-dir"});
  set_aliases("--output", {"-o"});
  set_aliases("--line", {"-l"});
  set_aliases("--skip", {"-s"});
  set_aliases("--length", {"-len"});
};

void Delete::set_parametrs() {
  try {
    Path = get_arg_value(0);
    Line = get<int>(get_key_value("--line"));
    Skip = get<int>(get_key_value("--skip"));
    Length = get<int>(get_key_value("--length"));
    Output = get<bool>(get_key_value("--output"));
    Directory = get<bool>(get_key_value("--directory"));
  } catch (const runtime_error &e) {
    throw_error(COMMAND_INCORRECT_PARAMETRS_EXCEPTION, e.what());
  }
};

void Delete::command_execution() const {
  try {
    SystemManager manager;
    if (Directory) {
      manager.delete_directory(Path);
    } else if (Line != -1) {
      manager.delete_data_from_file(Path, Line, Skip, Length);
    } else {
      manager.delete_file(Path);
    }
    if (Output) {
      string data = manager.read(Path);
      cout << data << endl;
    } else {
      cout << "Done." << endl;
    }
  } catch (const runtime_error &e) {
    write_error(COMMAND_EXECUTION_EXCEPTION, e.what());
  }
};