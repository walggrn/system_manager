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
            
            count_args = 2;

            keys["--recreate"] = { {}, {}, BOOLEAN, false };
            set_aliases("--recreate", {"-rc"});
        }

        void command_execution() const override {
            try{
                bool recreate = get<bool>(get_key_value("--recreate"));
                string path_from = get_arg_value(0);
                string path_to = get_arg_value(1);
                SystemManager manager;
                manager.modify(path_from, path_to, recreate);
            }
            catch(const exception& e){
                cerr << COMMAND_EXECUTION_EXCEPTION << "=> " << e.what() << endl;
            }
        }
};

#endif