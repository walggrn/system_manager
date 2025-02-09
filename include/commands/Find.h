#ifndef FIND_H
#define FIND_H

#include <string>

#include "commands/ICommand.h"
#include "utils/SystemManager.h"

using namespace std;

class Find : public ICommand {
private:
  std::string Name;
  std::string Path;
  bool Contains;
  TypeItem Type;
  int Limit;
  bool Recursive;
  int Depth;
  bool LogVisited;
  bool LogMatches;

public:
  Find();

  void set_parametrs() override;

  void command_execution() const override;
};

#endif