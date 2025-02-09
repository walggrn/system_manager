#include <format>
#include <iostream>
#include <map>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "commands/ICommand.h"
#include "messages/Exceptions.h"

using namespace std;

ArgData::ArgData() : name(""), default_value(nullopt), value(nullopt) {};

ArgData::ArgData(const string &name, const optional<string> default_value) {
  this->name = name;
  this->default_value = default_value;
  this->value = nullopt;
};

KeyData::KeyData()
    : required_keys({}), excluded_keys({}), type(UNKNOWN),
      default_value(nullptr), value(nullptr), is_set(false) {};

KeyData::KeyData(const Type &type, const KeyValue &default_value,
                 const set<string> &required_keys,
                 const set<string> &excluded_keys) {
  this->required_keys = required_keys;
  this->excluded_keys = excluded_keys;
  this->type = type;
  this->default_value = default_value;
  this->value = nullptr;
  this->is_set = false;
};

/// @brief получение руководства команды
/// @return руководство команды
string ICommand::get_guide() const { return guide; };

/// @brief сохранение всех имен ключа, которые ссылаются на основное
/// @param key основное имя ключа
/// @param aliases массив альтернативных имен
void ICommand::set_aliases(const string &key, const vector<string> aliases) {
  for (const auto &alias : aliases) {
    key_aliases[alias] = key;
  }
};

/// @brief получение аргумента
/// @param index индекс аргумента
/// @return значение аргумента
string ICommand::get_arg_value(const int index) const {
  if (index >= args.size()) {
    throw_error(LIMIT_ARGUMENT_EXCEPTION,
                format("Attempt to get value for argument but argument at "
                       "index: {} not exist.",
                       current_index_argument));
  }
  if (args[index].value.has_value()) {
    return args[index].value.value();
  } else {
    if (args[index].default_value.has_value()) {
      return args[index].default_value.value();
    } else {
      throw_error(
          ARGUMENT_MISSING_VALUE_EXCEPTION,
          format("Argument: \"{}\" hasn't been used.", args[index].name));
    }
  }
};

/// @brief получение значения ключа
/// @param key ключ
/// @return значение ключа
KeyValue ICommand::get_key_value(const string &key) const {
  auto it = get_key(key);
  if (!it.has_value()) {
    throw_error(
        UNKNOWN_KEY_EXCEPTION,
        format("Attempt to use unknown key: \"{}\" for command: \"{}\".", key,
               name));
  }
  const auto &key_data = keys.at(it.value());
  // значение было установлено
  if (key_data.is_set) {
    // проверка, что обязательные ключи установлены
    for (const auto &required_key : key_data.required_keys) {
      if (keys.at(required_key).is_set == false) {
        throw_error(KEY_USAGE_EXCEPTION,
                    format("Key \"{}\" requires key \"{}\" but ruquired key "
                           "hasn't been used.",
                           key, required_key));
      }
    }
    // проверка, что исключенные ключи не установлены
    for (const auto &excluded_key : key_data.excluded_keys) {
      if (keys.at(excluded_key).is_set) {
        throw_error(KEY_USAGE_EXCEPTION,
                    format("Key \"{}\" cannot be used with key \"{}\" but "
                           "excluded key has been used.",
                           key, excluded_key));
      }
    }
    return key_data.value;
  } else
    return key_data.default_value;
};

ICommand::ICommand() {};

/// @brief выполнение команды
void ICommand::execute() {
  if (get<bool>(get_key_value("--help")))
    cout << get_guide() << endl;
  else {
    try {
      set_parametrs();
      command_execution();
    } catch (const runtime_error &e) {
      cerr << e.what() << endl;
    }
  }
};

int ICommand::get_count_args() const { return args.size(); };

/// @brief получение основного имени ключа.
/// @param pseudo_key имя ключа.
/// @return имя основного ключа или nullopt.
optional<string> ICommand::get_key(const string &pseudo_key) const {
  if (keys.find(pseudo_key) != keys.end()) {
    return pseudo_key;
  }
  if (key_aliases.find(pseudo_key) != key_aliases.end())
    return key_aliases.at(pseudo_key);
  else {
    return nullopt;
  }
};

/// @brief проверка, что ключ является флагом
/// @param key ключ
/// @return ключ является флагом(true) - не является(false)
bool ICommand::is_simple_key(const string &key) const {
  auto it = get_key(key);
  if (it.has_value())
    return keys.at(it.value()).type == BOOLEAN;
  else {
    throw_error(
        UNKNOWN_KEY_EXCEPTION,
        format("Attempt to use unknown key: \"{}\" for command: \"{}\".", key,
               name));
  }
  return false;
};

/// @brief установить аргумент
/// @param value значение аргумента
void ICommand::set_arg_value(const string &value) {
  if (current_index_argument >= args.size()) {
    throw_error(LIMIT_ARGUMENT_EXCEPTION,
                format("Attempt to set value for argument but argument at "
                       "index: {} not exist.",
                       current_index_argument));
  }
  args[current_index_argument].value = value;
  current_index_argument++;
};

/// @brief установить значение ключа
/// @param key ключ
/// @param value значение(если ключ-флаг - игнорируется)
void ICommand::set_key_value(const string &key, const string &value) {
  auto it = get_key(key);
  if (it.has_value()) {
    if (keys[it.value()].is_set) {
      throw_error(
          KEY_USAGE_EXCEPTION,
          format(
              "Attempt to use key: \"{}\" but the key has already been used.",
              it.value()));
    }
    switch (keys[it.value()].type) {
    case INTEGER:
      int value_int;
      try {
        value_int = stoi(value);
      } catch (const invalid_argument &e) {
        throw_error(INVALID_KEY_VALUE_EXCEPTION,
                    format("Attempt to use non-integer value for key: \"{}\".",
                           it.value()));
      } catch (const out_of_range &e) {
        throw_error(INVALID_KEY_VALUE_EXCEPTION,
                    format("Value out of range for an integer key: \"{}\".",
                           it.value()));
      }
      keys[it.value()].value = value_int;
      keys[it.value()].is_set = true;
      break;
    case STRING:
      keys[it.value()].value = value;
      keys[it.value()].is_set = true;
      break;
    case BOOLEAN:
      keys[it.value()].value = true;
      keys[it.value()].is_set = true;
      break;
    default:
      break;
    }
  } else
    throw_error(
        UNKNOWN_KEY_EXCEPTION,
        format("Attempt to use unknown key: \"{}\" for command: \"{}\".", key,
               name));
};