#ifndef COPY_H
#define COPY_H

#include <string>
#include <map>
#include <iostream>
#include <stdexcept>

#include "ICommand.h"
#include "SystemManager.h"
#include "Exceptions.h"
#include "Guides.h"

using namespace std;

class Copy : public ICommand{
    public:
        Copy(){
            name = "copy";
            guide = COPY_GUIDE;

            args.push_back(ArgData("PATH1", nullopt));
            args.push_back(ArgData("PATH2", nullopt));

            keys["--t"] = KeyData(BOOLEAN, false);
            keys["--recreate"] = KeyData(BOOLEAN, false);
            keys["--directory"] = KeyData(BOOLEAN, false);
            keys["--recursive"] = KeyData(BOOLEAN, false, {"--directory"});
            set_aliases("--recreate", {"-rc"});
            set_aliases("--directory", {"-d"});
            set_aliases("--recursive", {"-r"});
        }
        
        void command_execution() const override {
            try{
                string path_from = get_arg_value(0);
                string path_to = get_arg_value(1);
                bool recreate = get<bool>(get_key_value("--recreate"));
                bool directory = get<bool>(get_key_value("--directory"));
                bool recursive = get<bool>(get_key_value("--recursive"));
                SystemManager manager;
                if(directory)
                    manager.copy_directory(path_from, path_to, recreate, recursive);
                else 
                    manager.copy_file(path_from, path_to, recreate);
                cout << "Done." << endl;
            }
            catch(const runtime_error& e){
                write_error(COMMAND_EXECUTION_EXCEPTION, e.what());
            }
        }
};

#endif