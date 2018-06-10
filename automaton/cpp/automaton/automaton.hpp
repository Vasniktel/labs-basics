#ifndef _AUTOMATON_H_
#define _AUTOMATON_H_

#include <vector>

class Automaton {
private:
  enum class State { STATE_1, STATE_2, STATE_3 };

  State state;
  State endState;

  State newState(char);

public:
  Automaton();
  bool read_commands(const std::vector<char>&);
};

#endif
