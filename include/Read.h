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

            args.push_back(ArgData("PATH", nullopt));

            keys["--detail"] = KeyData(BOOLEAN, false);
            set_aliases("--detail", {"-dt"});
        }
        
        void command_execution() const override {
            try{
                string path = get_arg_value(0);
                bool detail = get<bool>(get_key_value("--detail"));
                SystemManager manager;
                string data = manager.read(path, detail);
                cout << data << endl;
            }
            catch(const exception& e){
                write_error(COMMAND_EXECUTION_EXCEPTION, e.what());
            }
        }
};

#endif