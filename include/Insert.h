#ifndef INSERT_H
#define INSERT_H

#include <string>
#include <map>
#include <iostream>
#include <stdexcept>

#include "ICommand.h"
#include "SystemManager.h"
#include "Guides.h"

using namespace std;

class Insert : public ICommand{
    public:
        Insert(){
            name = "insert";
            guide = INSERT_GUIDE;
            
            args.push_back(ArgData("DATA", nullopt));
            args.push_back(ArgData("PATH", nullopt));

            keys["--output"] = KeyData(BOOLEAN, false, {"--line"});
            keys["--line"] = KeyData(INTEGER, -1);
            keys["--skip"] = KeyData(INTEGER, 0, {"--line"});
            set_aliases("--output", {"-o"});
            set_aliases("--line", {"-l"});
            set_aliases("--skip", {"-s"});
        }

        void command_execution() const override {
            try{
                string data = get_arg_value(0);
                string path = get_arg_value(1);
                int number_line = get<int>(get_key_value("--line"));
                int skip = get<int>(get_key_value("--skip"));
                bool output = get<bool>(get_key_value("--output"));
                SystemManager manager;
                manager.insert(path, data, number_line, skip);
                if(output)
                    cout << manager.read(path) << endl;
                else 
                    cout << "Done." << endl;
            }
            catch(const exception& e){
                write_error(COMMAND_EXECUTION_EXCEPTION, e.what());
            }
        }
};

#endif