#include <iostream>
#include <stdexcept>
#include <string>

#include "commands/ICommand.h"
#include "commands/Insert.h"
#include "messages/Guides.h"
#include "utils/SystemManager.h"

using namespace std;

/// @brief Настройка команды(имя, руководство, аргументы, ключи, аналогичные
/// имена для ключей)
Insert::Insert() {
  name = "insert";
  guide = INSERT_GUIDE;

  args.push_back(ArgData("DATA", nullopt));
  args.push_back(ArgData("PATH", nullopt));

  keys["--output"] = KeyData(BOOLEAN, false, {"--line"});
  keys["--line"] = KeyData(INTEGER, -1);
  keys["--skip"] = KeyData(INTEGER, 0, {"--line"});
  set_aliases("--output", {"-o"});
  set_aliases("--line", {"-l"});
  set_aliases("--skip", {"-s"});
};

/// @brief обработка параметров
void Insert::set_parametrs() {
  try {
    Data = get_arg_value(0);
    Path = get_arg_value(1);
    Line = get<int>(get_key_value("--line")) - 1;
    Skip = get<int>(get_key_value("--skip"));
    Output = get<bool>(get_key_value("--output"));
  } catch (const runtime_error &e) {
    throw_error(COMMAND_INCORRECT_PARAMETRS_EXCEPTION, e.what());
  }
};

/// @brief выполнение команды
void Insert::command_execution() const {
  try {
    SystemManager manager;
    manager.insert(Path, Data, Line, Skip);
    if (Output)
      cout << manager.read(Path) << endl;
    else
      cout << "Done." << endl;
  } catch (const exception &e) {
    write_error(COMMAND_EXECUTION_EXCEPTION, e.what());
  }
};