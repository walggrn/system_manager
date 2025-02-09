#ifndef COPY_H
#define COPY_H

#include <string>

#include "commands/ICommand.h"

class Copy : public ICommand {
private:
  std::string PathFrom;
  std::string PathTo;
  bool Recreate;
  bool Directory;
  bool Recursive;

public:
  Copy();

  void set_parametrs() override;

  void command_execution() const override;
};

#endif