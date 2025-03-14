#include <format>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "commands/ICommand.h"
#include "messages/Exceptions.h"
#include "utils/Parser.h"

using namespace std;

vector<string> Parser::parse_input(const string &input) {
  vector<string> tokens;
  stringstream ss(input);
  string token;
  bool in_quotes = false;
  for (size_t ind = 0; ind < input.size(); ind++) {
    char symbol = input[ind];
    if (symbol == '"') {
      in_quotes = !in_quotes;
      continue;
    }
    if (isspace(symbol) && !in_quotes) {
      if (!token.empty()) {
        tokens.push_back(token);
        token.clear();
      }
    } else
      token += symbol;
  }
  if (!token.empty())
    tokens.push_back(token);
  if (in_quotes) {
    throw_error(INPUT_EXCEPTION, "Unfinished expression in quotes.");
  }
  return tokens;
};

void Parser::parse_tokens(const vector<string> &tokens, ICommand &command) {
  for (size_t pos = 0; pos < tokens.size(); pos++) {
    string token = tokens[pos];
    optional<string> key;
    if ((key = command.get_key(token)).has_value()) {
      if (command.is_simple_key(key.value())) {
        if (key.value().compare("--help") == 0) {
          if (pos != 0) {
            cerr << HELP_KEY_IGNORED_WARNING << endl;
            continue;
          } else {
            if (tokens.size() > 1) {
              cerr << HELP_KEY_TYPED_WARNING << endl;
            }
            command.set_key_value("--help");
            return;
          }
        }
        command.set_key_value(key.value());
      } else if (pos + 1 == tokens.size()) {
        throw_error(MISSING_KEY_VALUE_EXCEPTION,
                    format("Key: \"{}\".", key.value()));
      } else {
        command.set_key_value(key.value(), tokens[pos + 1]);
        ++pos;
      }
    } else {
      if (command.get_count_args() <= pos) {
        throw_error(UNKNOWN_TOKEN_EXCEPTION, format("Token: \"{}\".", token));
      }
      command.set_arg_value(token);
    }
  }
};
