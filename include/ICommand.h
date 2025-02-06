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
#include <format>

#include "Exceptions.h"

using namespace std;
using KeyValue = variant<int, bool, string, nullptr_t>;

/// @brief тип значения ключа
enum Type{
    BOOLEAN,
    INTEGER,
    STRING,
    DOUBLE,
    UNKNOWN,
};

/// @brief хранение данных аргумента
struct ArgData{
    string name;
    optional<string> default_value;
    optional<string> value;

    ArgData() : name(""), default_value(nullopt), value(nullopt) {}

    /// @brief конструктор инициализации данных агрумента
    /// @param name имя аргумента
    /// @param default_value значение по умолчанию
    ArgData(const string& name, const optional<string> default_value) {
        this->name = name;
        this->default_value = default_value;
        this->value = nullopt;
    }
};

/// @brief хранение данных ключа
struct KeyData{
    set<string> required_keys; //обязательные ключи для использования
    set<string> excluded_keys; //исключающие ключи для использования
    Type type; //тип значения ключа
    KeyValue default_value; //значение по умолчанию
    KeyValue value; //установленное значение
    bool is_set; //установлено ли значение

    KeyData() : required_keys({}), excluded_keys({}), type(UNKNOWN), default_value(nullptr), value(nullptr), is_set(false) {}

    /// @brief создание данных ключа
    /// @param required_keys обязательные ключи для использования
    /// @param excluded_keys исключающие использование ключи
    /// @param type тип значения ключа
    /// @param default_value значение по умолчанию
    KeyData(const Type& type, 
            const KeyValue& default_value,
            const set<string>& required_keys = {}, 
            const set<string>& excluded_keys = {}){
        this->required_keys = required_keys;
        this->excluded_keys = excluded_keys;
        this->type = type;
        this->default_value = default_value;
        this->value = nullptr;
        this->is_set = false;
    }
};


/// @brief интерфейс для команд 
class ICommand {
    protected:
        string name;
        string guide;

        vector<ArgData> args;
        int current_index_argument = 0;
        
        //ключи команды
        map<string, KeyData> keys = {
            {"--help", KeyData(BOOLEAN, false)},
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

        virtual int get_count_args() const {
            return args.size();
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

        /// @brief проверка, что ключ является флагом
        /// @param key ключ
        /// @return ключ является флагом(true) - не является(false)
        virtual bool is_simple_key(const string& key) const {
            auto it = get_key(key);
            if(it.has_value())
                return keys.at(it.value()).type == BOOLEAN;
            else{
                throw_error(UNKNOWN_KEY_EXCEPTION, format("Attempt to use unknown key: \"{}\" for command: \"{}\".", key, name));
            }
        }

        /// @brief получение аргумента
        /// @param index индекс аргумента
        /// @return значение аргумента
        virtual string get_arg_value(const int index) const {
            if(index >= args.size())
            {
                throw_error(LIMIT_ARGUMENT_EXCEPTION, format("Attempt to get value for argument but argument at index: {} not exist.", current_index_argument));
            }
            if(args[index].value.has_value()){
                return args[index].value.value();
            }
            else{
                if(args[index].default_value.has_value()){
                    return args[index].default_value.value();
                }
                else{
                    throw_error(ARGUMENT_MISSING_VALUE_EXCEPTION, format("Argument: \"{}\" hasn't been used.", args[index].name));
                }
            }
        }

        /// @brief установить аргумент
        /// @param value значение аргумента
        virtual void set_arg_value(const string& value){
            if(current_index_argument >= args.size()){
                throw_error(LIMIT_ARGUMENT_EXCEPTION, format("Attempt to set value for argument but argument at index: {} not exist.", current_index_argument));
            }
            args[current_index_argument].value = value;
            current_index_argument++;
        }

        /// @brief получение значения ключа
        /// @param key ключ
        /// @return значение ключа
        virtual KeyValue get_key_value(const string& key) const {
            auto it = get_key(key);
            if (!it.has_value()) {
                throw_error(UNKNOWN_KEY_EXCEPTION, format("Attempt to use unknown key: \"{}\" for command: \"{}\".", key, name));
            }
            const auto& key_data = keys.at(it.value());
            //значение было установлено
            if(key_data.is_set){
                //проверка, что обязательные ключи установлены
                for (const auto& required_key : key_data.required_keys) {
                    if (keys.at(required_key).is_set == false) {
                        throw_error(KEY_USAGE_EXCEPTION, format("Key \"{}\" requires key \"{}\" but ruquired key hasn't been used.", key, required_key));
                    }
                }
                //проверка, что исключенные ключи не установлены
                for (const auto& excluded_key : key_data.excluded_keys) {
                    if (keys.at(excluded_key).is_set) {
                        throw_error(KEY_USAGE_EXCEPTION, format("Key \"{}\" cannot be used with key \"{}\" but excluded key has been used.", key, excluded_key));
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
                    throw_error(KEY_USAGE_EXCEPTION, format("Attempt to use key: \"{}\" but the key has already been used.", it.value()));
                }
                switch (keys[it.value()].type)
                {
                    case INTEGER:
                        int value_int;
                        try {
                            value_int = stoi(value);
                        }
                        catch (const invalid_argument& e) {
                            throw_error(INVALID_KEY_VALUE_EXCEPTION, format("Attempt to use non-integer value for key: \"{}\".",  it.value()));
                        }
                        catch (const out_of_range& e) {
                            throw_error(INVALID_KEY_VALUE_EXCEPTION, format("Value out of range for an integer key: \"{}\".", it.value()));
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
                throw_error(UNKNOWN_KEY_EXCEPTION, format("Attempt to use unknown key: \"{}\" for command: \"{}\".", key, name));
        }

        virtual ~ICommand() = default;
};

#endif