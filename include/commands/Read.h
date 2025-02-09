#ifndef READ_H
#define READ_H

#include <string>

#include "commands/ICommand.h"

using namespace std;

class Read : public ICommand {
private:
  std::string Path;
  bool Detail;

public:
  Read();

  void set_parametrs() override;

  void command_execution() const override;
};

#endif