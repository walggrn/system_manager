#ifndef RENAME_H
#define RENAME_H

#include <string>

#include "commands/ICommand.h"

class Modify : public ICommand {
private:
  std::string CurrentPath;
  std::string NewPath;
  bool Recreate;

public:
  Modify();

  void set_parametrs() override;

  void command_execution() const override;
};

#endif