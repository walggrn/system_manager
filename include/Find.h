#ifndef FIND_H
#define FIND_H

#include <chrono>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>

#include "Exceptions.h"
#include "Guides.h"
#include "ICommand.h"
#include "Log.h"
#include "SystemManager.h"

using namespace std;

class Find : public ICommand {
private:
  string Name;
  string Path;
  bool Contains;
  TypeItem Type;
  int Limit;
  bool Recursive;
  int Depth;
  bool LogVisited;
  bool LogMatches;

public:
  Find() {
    name = "find";
    guide = FIND_GUIDE;

    args.push_back(ArgData("NAME", nullopt));
    args.push_back(ArgData("PATH", "."));

    keys["--contains"] = KeyData(BOOLEAN, false);
    keys["--directory"] = KeyData(BOOLEAN, false, {}, {"--file", "--symlink"});
    keys["--file"] = KeyData(BOOLEAN, false, {}, {"--directory", "--symlink"});
    keys["--symlink"] = KeyData(BOOLEAN, false, {}, {"--directory", "--file"});
    keys["--recursive"] = KeyData(BOOLEAN, false);
    keys["--limit"] = KeyData(INTEGER, -1);
    keys["--depth"] = KeyData(INTEGER, -1, {"--recursive"});
    keys["--log-visited"] = KeyData(BOOLEAN, false, {"--recursive"});
    keys["--log-matches"] = KeyData(BOOLEAN, false, {"--recursive"});
    set_aliases("--contains", {"-c"});
    set_aliases("--directory", {"-dir"});
    set_aliases("--file", {"-f"});
    set_aliases("--symlink", {"-sm"});
    set_aliases("--recursive", {"-r"});
    set_aliases("--limit", {"-lim"});
    set_aliases("--depth", {"-d"});
    set_aliases("--log-visited", {"-lv"});
    set_aliases("--log-matches", {"-lm"});
  }

  void set_parametrs() override {
    try {
      Name = get_arg_value(0);
      Path = get_arg_value(1);
      Contains = get<bool>(get_key_value("--contains"));
      Type =
          get<bool>(get_key_value("--directory"))
              ? Directory
              : (get<bool>(get_key_value("--file"))
                     ? File
                     : (get<bool>(get_key_value("--symlink")) ? Symlink : All));
      Limit = get<int>(get_key_value("--limit"));
      Recursive = get<bool>(get_key_value("--recursive"));
      Depth = get<int>(get_key_value("--depth"));
      LogVisited = get<bool>(get_key_value("--log-visited"));
      LogMatches = get<bool>(get_key_value("--log-matches"));
    } catch (const runtime_error &e) {
      throw_error(COMMAND_INCORRECT_PARAMETRS_EXCEPTION, e.what());
    }
  }

  void command_execution() const override {
    try {
      vector<string> matches;
      vector<string> visited;
      SystemManager manager;
      manager.find(matches, visited, Name, Path, Contains, Type, Limit,
                   Recursive, Depth, LogVisited);
      if (LogVisited) {
        Log log = Log("visited");
        for (const auto &visit : visited)
          log.write(visit);
        cout << "Visited directories was saved at \"" << log.get_filename()
             << "\"." << endl;
      }
      if (LogMatches) {
        Log log = Log("matches");
        for (const auto &match : matches)
          log.write(match);
        cout << "Matches was saved at \"" << log.get_filename() << "\"."
             << endl;
      } else {
        for (const auto &match : matches)
          cout << match << endl;
      }
    } catch (const exception &e) {
      write_error(COMMAND_EXECUTION_EXCEPTION, e.what());
    }
  }
};

#endif