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
             
            count_args = 2;

            keys["--directory"] = { {}, {"--file", "--symlink"}, BOOLEAN, false };
            keys["--file"] = { {}, {"--directory", "--symlink"}, BOOLEAN, false };
            keys["--symlink"] = { {}, {"--directory", "--file"}, BOOLEAN, false };
            keys["--recursive"] = { {}, {}, BOOLEAN, false };
            keys["--limit"] = { {}, {}, INTEGER, -1 };
            keys["--depth"] = { {"-recursive"}, {}, INTEGER, -1 };
            keys["--log-visited"] = { {"-recursive"}, {}, BOOLEAN, false };
            keys["--log-matches"] = { {"-recursive"}, {}, BOOLEAN, false };
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
                bool directory = get<bool>(get_key_value("--directory"));
                bool file = get<bool>(get_key_value("--file"));
                bool link = get<bool>(get_key_value("--symlink"));
                TypeItem type = directory ? Directory : (file ? File : (link ? Symlink : All));
                bool recursive = get<bool>(get_key_value("--recursive"));
                int limit = get<int>(get_key_value("--limit"));
                int depth = get<int>(get_key_value("--depth"));
                bool log_visited = get<bool>(get_key_value("--log-visited"));
                bool log_matches = get<bool>(get_key_value("--log-matches"));
                SystemManager manager;
                vector<string> matches;
                vector<string> visited;
                string what = get_arg_value(0);
                string where = get_arg_value(1);
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
                cerr << COMMAND_EXECUTION_EXCEPTION << "=> " << e.what() << endl;
            }
        }
};

#endif