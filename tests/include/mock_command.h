#include <string>
#include <tuple>

#include "commands/ICommand.h"

using namespace std;

class MockCommand : public ICommand {
public:
  MockCommand();

  void set_parametrs() override;

  void command_execution() const override;

  tuple<string, string> get_args() const;

  tuple<bool, string, int> get_keys() const;
};