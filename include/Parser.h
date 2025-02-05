#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>

#include "ICommand.h"
#include "Exceptions.h"

/// @brief класс для парсинга ввода пользователя
class Parser{
    public:
        /// @brief разбитие строки на пробелы, учитывая кавычки(пробелы внутри кавычек не разбиваются)
        /// @param input строка
        /// @return массив токенов
        vector<string> parse_input(const string& input){
            vector<string> tokens;
            stringstream ss(input); 
            string token;
            bool in_quotes = false;
            for(size_t ind = 0; ind < input.size(); ind++){
                char symbol = input[ind];
                if(symbol == '"'){
                    in_quotes = !in_quotes;
                    continue;
                }
                if(isspace(symbol) && !in_quotes){
                    if(!token.empty())
                    {
                        tokens.push_back(token);
                        token.clear();
                    }
                }
                else 
                    token += symbol;
            }
            if(!token.empty())
                tokens.push_back(token);
            if(in_quotes){
                throw runtime_error(INPUT_EXCEPTION + "Unfinished expression in quotes.");
            }
            return tokens;
        }

        /// @brief обработка токенов на соответствие ключей или аргументов
        /// @param tokens массив токенов
        /// @param command команда
        void parse_tokens(const vector<string>& tokens,
                        ICommand& command){
            for(size_t pos = 1; pos < tokens.size(); pos++){
                string token = tokens[pos];
                //TODO реализовать более практичное использование --help
                if(token.compare("--help") == 0 || token.compare("-h") == 0){
                    if(pos != 1){
                        cerr << HELP_KEY_IGNORED_WARNING << endl;
                        continue;
                    }
                    else{
                        if(tokens.size() > 2){
                            cerr << HELP_KEY_TYPED_WARNING << endl;
                        }
                        command.set_key_value("--help");
                        return;
                    }           
                }
                if(pos < command.get_first_key_pos())
                    command.set_arg_value(token);
                else{
                    if(command.is_command_key(token)){
                        if(command.is_simple_key(token))
                            command.set_key_value(token);
                        else if(pos + 1 == tokens.size()){
                            throw runtime_error(MISSING_KEY_VALUE_EXCEPTION + "Key: \"" + token + "\".");
                        }
                        else{
                            command.set_key_value(token, tokens[pos + 1]);
                            ++pos;
                        }
                    }
                    else 
                        throw runtime_error(UNKNOWN_KEY_EXCEPTION + "Key: \"" + token + "\".");
                }
            }
        }
};

#endif