#include "mock_command.h"

MockCommand::MockCommand() {
  name = "test_command";
  guide = "GUIDE";

  args.push_back(ArgData("ARG1", nullopt));
  args.push_back(ArgData("ARG2", "default"));

  keys["--key1"] = KeyData(BOOLEAN, false);
  keys["--key2"] = KeyData(STRING, nullptr);
  keys["--key3"] = KeyData(INTEGER, nullptr);
  set_aliases("--key1", {"-k1"});
  set_aliases("--key2", {"-k2"});
  set_aliases("--key3", {"-k3"});
}

void MockCommand::set_parametrs() {}

void MockCommand::command_execution() const {}

tuple<string, string> MockCommand::get_args() const {
  return make_tuple(get_arg_value(0), get_arg_value(1));
}

tuple<bool, string, int> MockCommand::get_keys() const {
  return make_tuple(get<bool>(get_key_value("--key1")),
                    get<string>(get_key_value("--key2")),
                    get<int>(get_key_value("--key3")));
}