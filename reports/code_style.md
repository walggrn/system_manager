# Отчет "Стили кодирования"

## Выбранный стиль кодирования
В качестве стиля кодирования был выбран **LLVM**. Для форматирования использовался **clang-format**.

## Исходный код
```
#include <Create.h>
#include <Read.h>
#include <Copy.h>
#include <Insert.h>
#include <Delete.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <set>
using namespace std;

vector<string> parse_input(const string& input){
    vector<string> tokens;
    stringstream ss(input); 
    string token;
    bool in_quotes = false;
    string temp;
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
    if(in_quotes)
        cout << "Error: " << endl;
    return tokens;
}

map<string, int> parse_args(const vector<string>& tokens, const set<string>& keys_with_value, const set<string>& keys_without_value, const int start_pos){
    map<string, int> args;
    for(int i = start_pos; i < tokens.size(); i++){
        string key = tokens[i];
        if(keys_without_value.find(key) != keys_without_value.end())
            args[key] = 1;
        if(keys_with_value.find(key) != keys_with_value.end()){
            if (i + 1 < tokens.size()) {
                try{
                    args[key] = stoi(tokens[i + 1]);
                }
                catch (const invalid_argument& e) {
                    cout << "Error: wrong key value. Key value must be number" << endl;
                } catch (const out_of_range& e) {
                    cout << "Error: wrong key value. Key value is more than max value int" << endl;
                }
                ++i;
            } 
            else {
                cout << "Missing value for key " << key << endl;
            }
        }
        else{
            cout << "Wrong using command. Type \"info COMMAND_NAME\" for details using." << endl;
        }
    }
    return args;
}

void handle_info(vector<string>& command){
    if(command.size() < 2){
        cout << "Wrong using command: info. Type \"info COMMAND_NAME\"" << endl;
        return;
    }
    string command_to_info = command[1];
    if(command_to_info.compare("read") == 0){
        Read read_command;
        cout << read_command.get_info() << endl;
    }
    else if(command_to_info.compare("copy") == 0){
        Copy copy_command;
        cout << copy_command.get_info() << endl;
    }
    else if(command_to_info.compare("insert") == 0){
        Insert insert_command;
        cout << insert_command.get_info() << endl;
    }
    else if(command_to_info.compare("delete") == 0){
        Delete delete_command;
        cout << delete_command.get_info() << endl;
    }
    else if(command_to_info.compare("info") == 0)
        cout << "info COMMAND_NAME: output info about command and how to use it" << endl;
    else if(command_to_info.compare("list") == 0)
        cout << "list: output list commands" << endl;
    else
        cout << "Command: " <<  command_to_info << " not found. Type \"list\" to see list commands." << endl;
}

void handle_create(vector<string>& command){
    if(command.size() < 2)
    {
        cout << "Wrong using command: create. Type \"info create\" for details using." << endl;
        return;
    }
    Create create_command;
    map <string, int> args;
    create_command.execute(command[1], args);
}

void handle_read(vector<string>& command){
    if(command.size() < 2)
    {
        cout << "Wrong using command: read. Type \"info read\" for details using." << endl;
        return;
    }
    map<string, int> args;
    if (command.size() > 2)
        args = parse_args(command, {}, {"-d"}, 2);
    Read read_command;
    read_command.execute(command[1], args);
}

void handle_insert(vector<string>& command){
    if(command.size() < 3)
    {
        cout << "Wrong using command: insert. Type \"info insert\" for details using." << endl;
        return;
    }
    map<string, int> args;
    if (command.size() > 3)
        args = parse_args(command, {"-s", "-l"}, {"-o"}, 3);
    Insert insert_command;
    insert_command.execute(command[1], command[2], args);
}

void handle_copy(vector<string>& command){
    if(command.size() < 3)
    {
        cout << "Wrong using command: copy. Type \"info copy\" for details using." << endl;
        return;
    }
    map<string, int> args;
    Copy copy_command;
    copy_command.execute(command[1], command[2], args);
}

void handle_delete(vector<string>& command){
    if(command.size() < 2)
    {
        cout << "Wrong using command: delete. Type \"info delete\" for details using." << endl;
        return;
    }
    map<string, int> args;
    if (command.size() > 2)
        args = parse_args(command, {"-s", "-l"}, {}, 2);
    Delete delete_command;
    delete_command.execute(command[1], args);
}

int main(){
    string input;
    string list_command = "Commands:\nlist: output list command\ninfo COMMAND: output info of COMMAND\ncreate PATH_TO_FILE\nread PATH_TO_FILE\ncopy PATH_TO_FILE_COPY PATH_TO_FILE_INSERT_COPY\ninsert PATH_TO_FILE DATA_TO_INSERT\ndelete PATH_TO_FILE\nexit";
    cout << list_command << endl;
    while(true){
        cout << "> ";
        getline(cin, input);
        vector<string> command = parse_input(input);
        if(command.size() < 1)
            continue;
        string cmd = command[0];
        if(cmd.compare("list") == 0)
            cout << list_command << endl;
        else if(cmd.compare("info") == 0)
            handle_info(command);
        else if(cmd.compare("create") == 0)
            handle_create(command);
        else if(cmd.compare("read") == 0)
            handle_read(command);
        else if(cmd.compare("copy") == 0)
            handle_copy(command);
        else if(cmd.compare("insert") == 0)
            handle_insert(command);
        else if(cmd.compare("delete") == 0)
            handle_delete(command);
        else if(cmd.compare("exit") == 0)
            break;
        else
            cout << "The command don't found, type \"list\" to see list commands" << endl;
    }
    return 0;
}
```

## Отформатированный код
```
#include <Copy.h>
#include <Create.h>
#include <Delete.h>
#include <Insert.h>
#include <Read.h>

#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

vector<string> parse_input(const string &input) {
  vector<string> tokens;
  stringstream ss(input);
  string token;
  bool in_quotes = false;
  string temp;
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
  if (in_quotes)
    cout << "Error: " << endl;
  return tokens;
}

map<string, int> parse_args(const vector<string> &tokens,
                            const set<string> &keys_with_value,
                            const set<string> &keys_without_value,
                            const int start_pos) {
  map<string, int> args;
  for (int i = start_pos; i < tokens.size(); i++) {
    string key = tokens[i];
    if (keys_without_value.find(key) != keys_without_value.end())
      args[key] = 1;
    if (keys_with_value.find(key) != keys_with_value.end()) {
      if (i + 1 < tokens.size()) {
        try {
          args[key] = stoi(tokens[i + 1]);
        } catch (const invalid_argument &e) {
          cout << "Error: wrong key value. Key value must be number" << endl;
        } catch (const out_of_range &e) {
          cout << "Error: wrong key value. Key value is more than max value int"
               << endl;
        }
        ++i;
      } else {
        cout << "Missing value for key " << key << endl;
      }
    } else {
      cout << "Wrong using command. Type \"info COMMAND_NAME\" for details "
              "using."
           << endl;
    }
  }
  return args;
}

void handle_info(vector<string> &command) {
  if (command.size() < 2) {
    cout << "Wrong using command: info. Type \"info COMMAND_NAME\"" << endl;
    return;
  }
  string command_to_info = command[1];
  if (command_to_info.compare("read") == 0) {
    Read read_command;
    cout << read_command.get_info() << endl;
  } else if (command_to_info.compare("copy") == 0) {
    Copy copy_command;
    cout << copy_command.get_info() << endl;
  } else if (command_to_info.compare("insert") == 0) {
    Insert insert_command;
    cout << insert_command.get_info() << endl;
  } else if (command_to_info.compare("delete") == 0) {
    Delete delete_command;
    cout << delete_command.get_info() << endl;
  } else if (command_to_info.compare("info") == 0)
    cout << "info COMMAND_NAME: output info about command and how to use it"
         << endl;
  else if (command_to_info.compare("list") == 0)
    cout << "list: output list commands" << endl;
  else
    cout << "Command: " << command_to_info
         << " not found. Type \"list\" to see list commands." << endl;
}

void handle_create(vector<string> &command) {
  if (command.size() < 2) {
    cout << "Wrong using command: create. Type \"info create\" for details "
            "using."
         << endl;
    return;
  }
  Create create_command;
  map<string, int> args;
  create_command.execute(command[1], args);
}

void handle_read(vector<string> &command) {
  if (command.size() < 2) {
    cout << "Wrong using command: read. Type \"info read\" for details using."
         << endl;
    return;
  }
  map<string, int> args;
  if (command.size() > 2)
    args = parse_args(command, {}, {"-d"}, 2);
  Read read_command;
  read_command.execute(command[1], args);
}

void handle_insert(vector<string> &command) {
  if (command.size() < 3) {
    cout << "Wrong using command: insert. Type \"info insert\" for details "
            "using."
         << endl;
    return;
  }
  map<string, int> args;
  if (command.size() > 3)
    args = parse_args(command, {"-s", "-l"}, {"-o"}, 3);
  Insert insert_command;
  insert_command.execute(command[1], command[2], args);
}

void handle_copy(vector<string> &command) {
  if (command.size() < 3) {
    cout << "Wrong using command: copy. Type \"info copy\" for details using."
         << endl;
    return;
  }
  map<string, int> args;
  Copy copy_command;
  copy_command.execute(command[1], command[2], args);
}

void handle_delete(vector<string> &command) {
  if (command.size() < 2) {
    cout << "Wrong using command: delete. Type \"info delete\" for details "
            "using."
         << endl;
    return;
  }
  map<string, int> args;
  if (command.size() > 2)
    args = parse_args(command, {"-s", "-l"}, {}, 2);
  Delete delete_command;
  delete_command.execute(command[1], args);
}

int main() {
  string input;
  string list_command =
      "Commands:\nlist: output list command\ninfo COMMAND: output info of "
      "COMMAND\ncreate PATH_TO_FILE\nread PATH_TO_FILE\ncopy PATH_TO_FILE_COPY "
      "PATH_TO_FILE_INSERT_COPY\ninsert PATH_TO_FILE DATA_TO_INSERT\ndelete "
      "PATH_TO_FILE\nexit";
  cout << list_command << endl;
  while (true) {
    cout << "> ";
    getline(cin, input);
    vector<string> command = parse_input(input);
    if (command.size() < 1)
      continue;
    string cmd = command[0];
    if (cmd.compare("list") == 0)
      cout << list_command << endl;
    else if (cmd.compare("info") == 0)
      handle_info(command);
    else if (cmd.compare("create") == 0)
      handle_create(command);
    else if (cmd.compare("read") == 0)
      handle_read(command);
    else if (cmd.compare("copy") == 0)
      handle_copy(command);
    else if (cmd.compare("insert") == 0)
      handle_insert(command);
    else if (cmd.compare("delete") == 0)
      handle_delete(command);
    else if (cmd.compare("exit") == 0)
      break;
    else
      cout << "The command don't found, type \"list\" to see list commands"
           << endl;
  }
  return 0;
}
```

## Результат форматирования.
После форматирования код стал читабельнее. Импорт библиотек стал располагаться в алфавитном порядке, аргументы функций идут построчно, строка, добавлены переводы строки для строк, не вмещающихся в экран и тд..