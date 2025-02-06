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

            args.push_back(ArgData("PATH", nullopt));

            keys["--directory"] = KeyData(BOOLEAN, false, {}, {"--output", "--line", "--skip", "--length"});
            keys["--output"] = KeyData(BOOLEAN, false, {"--line"}, {"--directory"});
            keys["--line"] = KeyData(INTEGER, -1, {}, {"--directory"});
            keys["--skip"] = KeyData(INTEGER, 0, {"--line"}, {"--directory"});
            keys["--length"] = KeyData(INTEGER, -1, {"--line"}, {"--directory"});
            set_aliases("--directory", {"-dir"});
            set_aliases("--output", {"-o"});
            set_aliases("--line", {"-l"});
            set_aliases("--skip", {"-s"});
            set_aliases("--length",  {"-len"});
        }
        
        void command_execution() const override {
            try{
                string path = get_arg_value(0);
                bool directory = get<bool>(get_key_value("--directory"));
                bool output = get<bool>(get_key_value("--output"));
                int number_line = get<int>(get_key_value("--line"));
                int skip = get<int>(get_key_value("--skip"));
                int length = get<int>(get_key_value("--length"));
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
                write_error(COMMAND_EXECUTION_EXCEPTION, e.what());
            }
        }
};

#endif