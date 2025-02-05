#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <stdexcept>
#include <memory>
#include <functional>

#include "Parser.h"
#include "Copy.h"
#include "Create.h"
#include "Delete.h"
#include "Find.h"
#include "Insert.h"
#include "Modify.h"
#include "Read.h"
#include "Dir.h"
#include "Exceptions.h"

using namespace std;

//парсер ввода
Parser parser;
//мэп команд
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
void handle_command(vector<string>& tokens, shared_ptr<ICommand> command){
    try{
        parser.parse_tokens(tokens, *command);
    }
    catch(const exception& e){
        cerr << INPUT_EXCEPTION << "=> " << e.what() << endl;
        return;
    }
    command->execute();
}
 
int main(){
    string initial = "Welcome to the Console Manager!\n"
                    "This tool helps you manage files and directories directly from the terminal.\n"
                    "Type \"help\" to see detailed usage instructions, or \"list\" to view all available commands.\n"
                    "If you're unsure about how to use a specific command, try \"command_name --help\" for guidance.";
    string guide = "Console File Manager - Help\n"
                    "\n"
                    "This tool supports the following commands:\n"
                    "\tlist                                                  Displays all available commands.\n"
                    "\tcreate                                                Creates a new file or directory. Use \"create --help\" for details.\n"
                    "\tmodify                                                Modifies an existing file or directory. Use \"modify --help\" for details.\n"
                    "\tfind                                                  Searches for a file or directory. Use \"find --help\" for details.\n"
                    "\tcopy                                                  Copies a file or directory. Use \"copy --help\" for details.\n"
                    "\tread                                                  Reads and displays file contents. Use \"read --help\" for details.\n"
                    "\tinsert                                                Inserts data into a file. Use \"insert --help\" for details.\n"
                    "\tdelete                                                Deletes a file or directory. Use \"delete --help\" for details.\n"
                    "\texit                                                  Exits the console manager.\n"
                    "\n"
                    "For more details on any command, type \"command_name --help\".";
    string list_commands = "Commands:\n"
                            "\tlist\n"
                            "\tcreate\n"
                            "\tmodify\n"
                            "\tfind\n"
                            "\tcopy\n"
                            "\tread\n"
                            "\tinsert\n"    
                            "\tdelete\n"
                            "\texit";
    cout << initial << endl;
    string input;

    //Считываем ввод пользователя
    while(true){
        cout << "> ";
        getline(cin, input);
        vector<string> tokens;
        try{
            tokens = parser.parse_input(input);
        }
        catch(const runtime_error& e){
            cerr << e.what() << endl;
            continue;
        }
        if(tokens.size() < 1)
            continue;
        string command_name = tokens[0];
        //проверяем имя команды
        auto it = Commands.find(command_name);
        if (it != Commands.end())
            handle_command(tokens, it->second());
        else if(command_name.compare("help") == 0)
            cout << guide << endl;
        else if(command_name.compare("list") == 0)
            cout << list_commands << endl;
        else if(command_name.compare("exit") == 0)
            break;
        else
            cout << INPUT_EXCEPTION << "Command: " << command_name << " don't found, type \"list\" to see a list of available commands" << endl;
    }
    return 0;
}