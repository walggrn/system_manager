#ifndef DELETE_H
#define DELETE_H

#include <string>
#include <map>
#include <iostream>
#include <stdexcept>

#include "ICommand.h"
#include "SystemManager.h"
#include "Exceptions.h"
#include "Guides.h"

using namespace std;

class Delete : public ICommand{
    public:
        Delete(){
            name = "delete";
            guide = DELETE_GUIDE;

            count_args = 1;

            keys["--directory"] = { {}, {"--output", "--line", "--skip", "--length"}, BOOLEAN, false };
            keys["--output"] = { {"--line"}, {"--directory"}, BOOLEAN, false };
            keys["--line"] = { {}, {"--directory"}, INTEGER, -1 };
            keys["--skip"] = { {"--line"}, {"--directory"}, INTEGER, 0 };
            keys["--length"] = { {"--line"}, {"--directory"}, INTEGER, -1 };
            set_aliases("--directory", {"-d"});
            set_aliases("--output", {"-o"});
            set_aliases("--line", {"-l"});
            set_aliases("--skip", {"-s"});
            set_aliases("--length",  {"-len"});
        }
        
        void command_execution() const override {
            try{
                bool directory = get<bool>(get_key_value("--directory"));
                bool output = get<bool>(get_key_value("--output"));
                int number_line = get<int>(get_key_value("--line"));
                int skip = get<int>(get_key_value("--skip"));
                int length = get<int>(get_key_value("--length"));
                string path = get_arg_value(0);
                SystemManager manager;
                if(directory)
                    manager.delete_directory(path);
                else if(number_line != -1)
                    manager.delete_data_from_file(path, number_line, skip, length);
                else 
                    manager.delete_file(path);
                if(output)
                {
                    string data = manager.read(path);
                    cout << data << endl;
                }
                else 
                    cout << "Done." << endl;
            }
            catch(const runtime_error& e){
                cerr << COMMAND_EXECUTION_EXCEPTION << "=> " << e.what() << endl;
            }
        }
};

#endif