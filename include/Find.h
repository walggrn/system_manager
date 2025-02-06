#ifndef FIND_H
#define FIND_H

#include <string>
#include <map>
#include <set>
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <iomanip>
#include <sstream>

#include "ICommand.h"
#include "SystemManager.h"
#include "Log.h"
#include "Exceptions.h"
#include "Guides.h"

using namespace std;

class Find : public ICommand{
    public:
        Find(){
            name = "find";
            guide = FIND_GUIDE;
             
            args.push_back(ArgData("NAME", nullopt));
            args.push_back(ArgData("PATH", "."));

            keys["--directory"] = KeyData(BOOLEAN, false, {}, {"--file", "--symlink"});
            keys["--file"] = KeyData(BOOLEAN, false, {}, {"--directory", "--symlink"});
            keys["--symlink"] = KeyData(BOOLEAN, false, {}, {"--directory", "--file"});
            keys["--recursive"] = KeyData(BOOLEAN, false);
            keys["--limit"] = KeyData(INTEGER, -1);
            keys["--depth"] = KeyData(INTEGER, -1, {"--recursive"});
            keys["--log-visited"] = KeyData(BOOLEAN, false, {"--recursive"});
            keys["--log-matches"] = KeyData(BOOLEAN, false, {"--recursive"});
            set_aliases("--directory", {"-dir"});
            set_aliases("--file", {"-f"});
            set_aliases("--symlink", {"-sm"});
            set_aliases("--recursive", {"-r"});
            set_aliases("--limit", {"-lim"});
            set_aliases("--depth", {"-d"});
            set_aliases("--log-visited", {"-lv"});
            set_aliases("--log-matches", {"-lm"});
        }

        void command_execution() const override {
            try{
                string what = get_arg_value(0);
                string where = get_arg_value(1);
                bool directory = get<bool>(get_key_value("--directory"));
                bool file = get<bool>(get_key_value("--file"));
                bool link = get<bool>(get_key_value("--symlink"));
                TypeItem type = directory ? Directory : (file ? File : (link ? Symlink : All));
                bool recursive = get<bool>(get_key_value("--recursive"));
                int limit = get<int>(get_key_value("--limit"));
                int depth = get<int>(get_key_value("--depth"));
                bool log_visited = get<bool>(get_key_value("--log-visited"));
                bool log_matches = get<bool>(get_key_value("--log-matches"));
                vector<string> matches;
                vector<string> visited {where};
                SystemManager manager;
                manager.find(matches, visited, what, where, type, recursive, limit, depth, log_visited);
                if(log_matches){
                    Log log = Log("matches");
                    for(const auto& match : matches)
                        log.write(match);
                    cout << "Matches was saved at \"" << log.get_filename() << "\"." << endl;
                }
                else{
                    for(const auto& match : matches)
                        cout << match << endl;
                }
                if(log_visited){
                    Log log = Log("visited");
                    for(const auto& visit : visited)
                        log.write(visit);
                    cout << "Visited directories was saved at \"" << log.get_filename() << "\"." << endl;
                }
            }
            catch(const exception& e){
                write_error(COMMAND_EXECUTION_EXCEPTION, e.what());
            }
        }
};

#endif