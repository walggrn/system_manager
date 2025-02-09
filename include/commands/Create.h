#ifndef CREATE_H
#define CREATE_H

#include <string>

#include "commands/ICommand.h"

class Create : public ICommand {
private:
  std::string Path;
  bool Recreate;
  bool Directory;

public:
  Create();

  void set_parametrs() override;

  void command_execution() const override;
};

#endif