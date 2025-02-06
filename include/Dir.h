#ifndef DIR_H
#define DIR_H

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <string>

#include "ICommand.h"
#include "SystemManager.h"
#include "Exceptions.h"
#include "Guides.h"

using namespace std;


class Dir : public ICommand{
    public:
        Dir(){
            name = "dir";
            guide = DIR_GUIDE;

            args.push_back(ArgData("PATH", "."));

            keys["--recursive"] = KeyData(BOOLEAN, false);
            keys["--depth"] = KeyData(INTEGER, -1, {"--recursive"});
            set_aliases("--recursive", {"-r"});
            set_aliases("--depth", {"-d"});
        }
        
        void command_execution() const override {
            try{
                string path = get_arg_value(0);
                bool recursive = get<bool>(get_key_value("--recursive"));
                int depth = get<int>(get_key_value("--depth"));
                SystemManager manager;
                manager.list(path, recursive, depth);
            }
            catch(const exception& e){
                write_error(COMMAND_EXECUTION_EXCEPTION, e.what());
            }
        }
};

#endif