#ifndef INSERT_H
#define INSERT_H

#include <string>

#include "commands/ICommand.h"

class Insert : public ICommand {
private:
  std::string Data;
  std::string Path;
  int Line;
  int Skip;
  bool Output;

public:
  Insert();

  void set_parametrs() override;

  void command_execution() const override;
};

#endif