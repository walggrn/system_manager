#ifndef CREATE_H
#define CREATE_H

#include <string>
#include <map>
#include <iostream>
#include <stdexcept>

#include "ICommand.h"
#include "SystemManager.h"
#include "Exceptions.h"
#include "Guides.h"

using namespace std;

class Create : public ICommand{
    public:
        Create(){
            name = "create";
            guide = CREATE_GUIDE;

            args.push_back(ArgData("PATH", nullopt));

            keys["--recreate"] = KeyData(BOOLEAN, false);
            keys["--directory"] = KeyData(BOOLEAN, false);
            set_aliases("--recreate", {"-rc"});
            set_aliases("--directory", {"-d"});
        }
        
        void command_execution() const override {
            try{
                string path = get_arg_value(0);
                bool recreate = get<bool>(get_key_value("--recreate"));
                bool directory = get<bool>(get_key_value("--directory"));
                SystemManager manager;
                if(directory)
                    manager.create_directory(path, recreate);
                else
                    manager.create_file(path, recreate);
                cout << "Done." << endl;
            }
            catch(const runtime_error& e){
                write_error(COMMAND_EXECUTION_EXCEPTION, e.what());
            }
        }
};

#endif