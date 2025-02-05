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

            count_args = 2;

            keys["--recreate"] = { {}, {}, BOOLEAN, false };
            keys["--directory"] = { {}, {}, BOOLEAN, false };
            keys["--recursive"] = { {"--directory"}, {}, BOOLEAN, false };
            set_aliases("--recreate", {"-rc"});
            set_aliases("--directory", {"-d"});
            set_aliases("--recursive", {"-r"});
        }
        
        void command_execution() const override {
            try{
                bool recreate = get<bool>(get_key_value("--recreate"));
                bool directory = get<bool>(get_key_value("--directory"));
                bool recursive = get<bool>(get_key_value("--recursive"));
                string path_from = get_arg_value(0);
                string path_to = get_arg_value(1);
                SystemManager manager;
                if(directory)
                    manager.copy_directory(path_from, path_to, recreate, recursive);
                else 
                    manager.copy_file(path_from, path_to, recreate);
                cout << "Done." << endl;
            }
            catch(const runtime_error& e){
                cerr << COMMAND_EXECUTION_EXCEPTION << "=> " << e.what() << endl;
            }
        }
};

#endif