#include <iostream>
#include <string>
#include <vector>

#include "commands/ICommand.h"
#include "commands/Modify.h"
#include "messages/Guides.h"
#include "utils/SystemManager.h"

using namespace std;

/// @brief Настройка команды(имя, руководство, аргументы, ключи, аналогичные
/// имена для ключей)
Modify::Modify() {
  name = "modify";
  guide = MODIFY_GUIDE;

  args.push_back(ArgData("PATH1", nullopt));
  args.push_back(ArgData("PATH2", nullopt));

  keys["--recreate"] = KeyData(BOOLEAN, false);
  set_aliases("--recreate", {"-rc"});
};

/// @brief обработка параметров
void Modify::set_parametrs() {
  try {
    CurrentPath = get_arg_value(0);
    NewPath = get_arg_value(1);
    Recreate = get<bool>(get_key_value("--recreate"));
  } catch (const runtime_error &e) {
    throw_error(COMMAND_INCORRECT_PARAMETRS_EXCEPTION, e.what());
  }
};

/// @brief выполнение команды
void Modify::command_execution() const {
  try {
    SystemManager manager;
    manager.modify(CurrentPath, NewPath, Recreate);
  } catch (const exception &e) {
    write_error(COMMAND_EXECUTION_EXCEPTION, e.what());
  }
};