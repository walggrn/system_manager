#ifndef RENAME_H
#define RENAME_H

#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "ICommand.h"
#include "SystemManager.h"
#include "Guides.h"

using namespace std;

class Modify : public ICommand{
    public:
        Modify(){
            name = "modify";
            guide = MODIFY_GUIDE;
            
            args.push_back(ArgData("PATH1", nullopt));
            args.push_back(ArgData("PATH2", nullopt));

            keys["--recreate"] = KeyData(BOOLEAN, false);
            set_aliases("--recreate", {"-rc"});
        }

        void command_execution() const override {
            try{
                string path_from = get_arg_value(0);
                string path_to = get_arg_value(1);
                bool recreate = get<bool>(get_key_value("--recreate"));
                SystemManager manager;
                manager.modify(path_from, path_to, recreate);
            }
            catch(const exception& e){
                write_error(COMMAND_EXECUTION_EXCEPTION, e.what());
            }
        }
};

#endif