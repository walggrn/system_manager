#ifndef DIR_H
#define DIR_H

#include <string>

#include "commands/ICommand.h"

using namespace std;

class Dir : public ICommand {
private:
  std::string Path;
  bool Recursive;
  bool Depth;

public:
  Dir();

  void set_parametrs() override;

  void command_execution() const override;
};

#endif