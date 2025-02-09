#ifndef PARSE_H
#define PARSE_H

#include <string>
#include <vector>

#include "commands/ICommand.h"

/// @brief класс для парсинга ввода пользователя
class Parser {
public:
  /// @brief разбитие строки на пробелы, учитывая кавычки(пробелы внутри кавычек
  /// не разбиваются)
  /// @param input строка
  /// @return массив токенов
  vector<string> parse_input(const string &input);

  /// @brief обработка токенов на соответствие ключей или аргументов
  /// @param tokens массив токенов
  /// @param command команда
  void parse_tokens(const vector<string> &tokens, ICommand &command);
};

#endif