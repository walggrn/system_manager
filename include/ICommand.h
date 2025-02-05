#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <string>
#include <map>
#include <iostream>
#include <stdexcept>
#include <variant>
#include <string>
#include <chrono>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <optional>

#include "Exceptions.h"

using namespace std;
using KeyValue = variant<int, bool, string, nullptr_t>;

/// @brief тип значения ключа
enum Type{
    BOOLEAN,
    INTEGER,
    STRING,
    DOUBLE,
};

/// @brief хранение данных ключа
struct KeyData{
    set<string> required_keys; //обязательные ключи для использования
    set<string> excluded_keys; //исключающие ключи для использования
    Type type; //тип значения ключа
    KeyValue default_value; //значение по умолчанию
    KeyValue value = nullptr; //установленное значение
    bool is_set = false; //установлено ли значение
};

/// @brief интерфейс для команд 
class ICommand {
    protected:
        string name;
        string guide;

        int count_args;
        vector<string> args_values;
        
        //ключи команды
        map<string, KeyData> keys = {
            {"--help", 
                { {}, {}, BOOLEAN, false },
            },
        };
        unordered_map<string, string> key_aliases;

    public:
        ICommand(){
            set_aliases("--help", {"-h"});
        }
        
        /// @brief выполнение команды
        virtual void execute() const {
            if(get<bool>(get_key_value("--help")))
                cout << get_guide() << endl;
            else 
                command_execution();
        }

        /// @brief получение значений ключей и вызов метода SystemManager
        virtual void command_execution() const = 0;

        /// @brief получение руководства команды
        /// @return руководство команды
        virtual string get_guide() const {
            return guide;
        }

        /// @brief сохранение всех имен ключа, которые ссылаются на основное
        /// @param key основное имя ключа
        /// @param aliases массив альтернативных имен
        virtual void set_aliases(const string& key, const vector<string> aliases) {
            for(const auto& alias : aliases){
                key_aliases[alias] = key;
            }
        }

        /// @brief получение основного имени ключа.
        /// @param pseudo_key имя ключа.
        /// @return имя основного ключа или nullopt.
        virtual optional<string> get_key(const string& pseudo_key) const {
            if(keys.find(pseudo_key) != keys.end()){
                return pseudo_key;
            }
            else{
                if(key_aliases.find(pseudo_key) != key_aliases.end())
                    return key_aliases.at(pseudo_key);
                else 
                    return nullopt;
            }
        }

        /// @brief получение начальной позиции ключей
        /// @return индекс первого ключа
        virtual int get_first_key_pos() const {
            return count_args + 1;
        }

        /// @brief проверка использования ключа командой
        /// @param key ключ
        /// @return команда использует ключ(true) - не использует(false)
        virtual bool is_command_key(const string& key) const {
            return get_key(key).has_value();
        }

        /// @brief проверка, что ключ является флагом
        /// @param key ключ
        /// @return ключ является флагом(true) - не является(false)
        virtual bool is_simple_key(const string& key) const {
            auto it = get_key(key);
            if(it.has_value())
                return keys.at(it.value()).type == BOOLEAN;
            else{
                throw runtime_error(UNKNOWN_KEY_EXCEPTION + "Key: \"" + key + "\".");
            }
        }

        /// @brief получение аргумента
        /// @param index индекс аргумента
        /// @return значение аргумента
        virtual string get_arg_value(const int index) const {
            if(index >= args_values.size())
            {
                throw runtime_error(ARGUMENT_NOT_FOUND_EXCEPTION + "Argument at index: " + to_string(index) + " not found.");
            }
            return args_values[index];
        }

        /// @brief установить аргумент
        /// @param value значение аргумента
        virtual void set_arg_value(const string& value){
            if(count_args < args_values.size()){
                throw runtime_error(LIMIT_ARGUMENT_EXCEPTION);
            }
            args_values.push_back(value);
        }

        /// @brief получение значения ключа
        /// @param key ключ
        /// @return значение ключа
        virtual KeyValue get_key_value(const string& key) const {
            auto it = get_key(key);
            if (!it.has_value()) {
                throw runtime_error(UNKNOWN_KEY_EXCEPTION + "Key: \"" + key + "\".");
            }
            const auto& key_data = keys.at(it.value());
            //значение было установлено
            if(key_data.is_set){
                //проверка, что обязательные ключи установлены
                for (const auto& required_key : key_data.required_keys) {
                    if (keys.at(required_key).is_set == false) {
                        throw runtime_error(KEY_USAGE_EXCEPTION + "Key \"" + key + "\" requires key \"" + required_key + "\".");
                    }
                }
                //проверка, что исключенные ключи не установлены
                for (const auto& excluded_key : key_data.excluded_keys) {
                    if (keys.at(excluded_key).is_set) {
                        throw runtime_error(KEY_USAGE_EXCEPTION + "Key \"" + key + "\" cannot be used with key \"" + excluded_key + "\".");
                    }
                }
                return key_data.value;
            }
            else 
                return key_data.default_value;
        }

        /// @brief установить значение ключа
        /// @param key ключ
        /// @param value значение(если ключ-флаг - игнорируется)
        virtual void set_key_value(const string& key, const string& value = "") {
            auto it = get_key(key);
            if(it.has_value()){
                if(keys[it.value()].is_set){
                    throw runtime_error(KEY_USAGE_EXCEPTION + "Reuse key: \"" + it.value() + "\".");
                }
                switch (keys[it.value()].type)
                {
                    case INTEGER:
                        int value_int;
                        try {
                            value_int = stoi(value);
                        }
                        catch (const invalid_argument& e) {
                            throw runtime_error(INVALID_KEY_VALUE_EXCEPTION + "Invalid argument provided for an integer key: \"" + it.value() + "\".");
                        }
                        catch (const out_of_range& e) {
                            throw runtime_error(INVALID_KEY_VALUE_EXCEPTION + "Value out of range for an integer key: \"" + it.value() + "\".");
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
            }
            else 
                throw runtime_error(UNKNOWN_KEY_EXCEPTION + "Key: \"" + key + "\".");
        }

        virtual ~ICommand() = default;
};

#endif