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
            
            count_args = 2;

            keys["--output"] = { {"--line"}, {}, BOOLEAN, false };
            keys["--line"] = { {}, {}, INTEGER, -1 };
            keys["--skip"] = { {"--line"}, {}, INTEGER, 0 };
            set_aliases("--output", {"-o"});
            set_aliases("--line", {"-l"});
            set_aliases("--skip", {"-s"});
        }

        void command_execution() const override {
            try{
                int number_line = get<int>(get_key_value("--line"));
                int skip = get<int>(get_key_value("--skip"));
                bool output = get<bool>(get_key_value("--output"));
                SystemManager manager;
                string path = get_arg_value(0);
                string data = get_arg_value(1);
                manager.insert(path, data, number_line, skip);
                if(output)
                    cout << manager.read(path) << endl;
                else 
                    cout << "Done." << endl;
            }
            catch(const exception& e){
                cerr << COMMAND_EXECUTION_EXCEPTION << "=> " << e.what() << endl;
            }
        }
};

#endif