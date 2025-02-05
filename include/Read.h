#ifndef READ_H
#define READ_H

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


class Read : public ICommand{
    public:
        Read(){
            name = "read";
            guide = READ_GUIDE;

            count_args = 1;

            keys["--detail"] = { {}, {}, BOOLEAN, false };
            set_aliases("--detail", {"-dt"});
        }
        
        void command_execution() const override {
            try{
                bool detail = get<bool>(get_key_value("--detail"));
                string path = get_arg_value(0);
                SystemManager manager;
                string data = manager.read(path, detail);
                cout << data << endl;
            }
            catch(const exception& e){
                cerr << COMMAND_EXECUTION_EXCEPTION << "=> " << e.what() << endl;
            }
        }
};

#endif