#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <chrono>
#include <format>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "messages/Exceptions.h"

using KeyValue = variant<int, bool, std::string, nullptr_t>;

/// @brief тип значения ключа
enum Type {
  BOOLEAN,
  INTEGER,
  STRING,
  DOUBLE,
  UNKNOWN,
};

/// @brief хранение данных аргумента
struct ArgData {
  std::string name;
  std::optional<std::string> default_value;
  std::optional<std::string> value;

  ArgData();

  /// @brief конструктор инициализации данных агрумента
  /// @param name имя аргумента
  /// @param default_value значение по умолчанию
  ArgData(const std::string &name,
          const std::optional<std::string> default_value);
};

/// @brief хранение данных ключа
struct KeyData {
  std::set<std::string> required_keys; // обязательные ключи для использования
  std::set<std::string> excluded_keys; // исключающие ключи для использования
  Type type;                           // тип значения ключа
  KeyValue default_value;              // значение по умолчанию
  KeyValue value;                      // установленное значение
  bool is_set;                         // установлено ли значение

  KeyData();

  /// @brief создание данных ключа
  /// @param required_keys обязательные ключи для использования
  /// @param excluded_keys исключающие использование ключи
  /// @param type тип значения ключа
  /// @param default_value значение по умолчанию
  KeyData(const Type &type, const KeyValue &default_value,
          const std::set<std::string> &required_keys = {},
          const std::set<std::string> &excluded_keys = {});
};

/// @brief интерфейс для команд
class ICommand {
protected:
  std::string name;
  std::string guide;
  std::vector<ArgData> args;
  int current_index_argument = 0;
  std::map<std::string, KeyData> keys = {
      {"--help", KeyData(BOOLEAN, false)},
  };
  std::unordered_map<std::string, std::string> key_aliases = {
      {"--help", "-h"},
  };

  /// @brief получение руководства команды
  /// @return руководство команды
  virtual std::string get_guide() const;

  /// @brief сохранение всех имен ключа, которые ссылаются на основное
  /// @param key основное имя ключа
  /// @param aliases массив альтернативных имен
  virtual void set_aliases(const std::string &key,
                           const std::vector<std::string> aliases);

  /// @brief получение аргумента
  /// @param index индекс аргумента
  /// @return значение аргумента
  virtual std::string get_arg_value(const int index) const;

  /// @brief получение значения ключа
  /// @param key ключ
  /// @return значение ключа
  virtual KeyValue get_key_value(const std::string &key) const;

  virtual void set_parametrs() = 0;

  /// @brief получение значений ключей и вызов метода SystemManager
  virtual void command_execution() const = 0;

public:
  ICommand();

  /// @brief выполнение команды
  virtual void execute();

  virtual int get_count_args() const;

  /// @brief получение основного имени ключа.
  /// @param pseudo_key имя ключа.
  /// @return имя основного ключа или nullopt.
  virtual std::optional<std::string>
  get_key(const std::string &pseudo_key) const;

  /// @brief проверка, что ключ является флагом
  /// @param key ключ
  /// @return ключ является флагом(true) - не является(false)
  virtual bool is_simple_key(const std::string &key) const;

  /// @brief установить аргумент
  /// @param value значение аргумента
  virtual void set_arg_value(const std::string &value);

  /// @brief установить значение ключа
  /// @param key ключ
  /// @param value значение(если ключ-флаг - игнорируется)
  virtual void set_key_value(const std::string &key,
                             const std::string &value = "");

  virtual ~ICommand() = default;
};

#endif