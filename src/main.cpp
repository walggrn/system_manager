#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "commands/Copy.h"
#include "commands/Create.h"
#include "commands/Delete.h"
#include "commands/Dir.h"
#include "commands/Find.h"
#include "commands/Insert.h"
#include "commands/Modify.h"
#include "commands/Read.h"
#include "messages/Exceptions.h"
#include "messages/Guides.h"
#include "utils/Parser.h"

using namespace std;

// парсер ввода
Parser parser;
// мэп команд
map<string, function<shared_ptr<ICommand>()>> Commands = {
    {"create", []() { return make_shared<Create>(); }},
    {"read", []() { return make_shared<Read>(); }},
    {"dir", []() { return make_shared<Dir>(); }},
    {"copy", []() { return make_shared<Copy>(); }},
    {"delete", []() { return make_shared<Delete>(); }},
    {"find", []() { return make_shared<Find>(); }},
    {"insert", []() { return make_shared<Insert>(); }},
    {"modify", []() { return make_shared<Modify>(); }},
};

/// @brief вызов команды
/// @param tokens массив токенов
/// @param command команда
void handle_command(vector<string> &tokens, shared_ptr<ICommand> command) {
  try {
    parser.parse_tokens(tokens, *command);
  } catch (const exception &e) {
    write_error(INPUT_EXCEPTION, e.what());
    return;
  }
  command->execute();
}

auto main() -> int {
  string initial = INITIAL_MESSAGE;
  string guide = GUIDE;
  string list_commands = LIST_COMMANDS;
  cout << initial << endl;
  string input;

  // Считываем ввод пользователя
  while (true) {
    cout << "> ";
    getline(cin, input);
    vector<string> tokens;
    try {
      tokens = parser.parse_input(input);
    } catch (const runtime_error &e) {
      cerr << e.what() << endl;
      continue;
    }
    if (tokens.size() < 1) {
      continue;
    }
    //1111
    //22222
    //33333
    string command_name = tokens[0];
    // проверяем имя команды
    auto command = Commands.find(command_name);
    vector<string> parametrs = vector<string>(tokens.begin() + 1, tokens.end());
    if (command != Commands.end()) {
      handle_command(parametrs, command->second());
    } else if (command_name == "help") {
      cout << guide << endl;
    } else if (command_name == "list") {
      cout << list_commands << endl;
    } else if (command_name == "exit") {
      break;
    } else {
      write_error(INPUT_EXCEPTION,
                  format("Command: \"{}\" don't found, type \"list\" to see a "
                         "list of available commands.",
                         command_name));
    }
  }
  return 0;
}