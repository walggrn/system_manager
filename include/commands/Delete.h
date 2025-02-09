#ifndef DELETE_H
#define DELETE_H

#include <string>

#include "commands/ICommand.h"

class Delete : public ICommand {
private:
  std::string Path;
  int Line;
  int Skip;
  int Length;
  bool Output;
  bool Directory;

public:
  Delete();

  void set_parametrs() override;

  void command_execution() const override;
};

#endif