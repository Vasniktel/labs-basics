#include <vector>
#include "automaton.hpp"

Automaton::Automaton() :
  state(Automaton::State::STATE_1),
  endState(Automaton::State::STATE_2)
{ }

bool Automaton::read_commands(const std::vector<char>& commands) {
  for (const char& command : commands) {
    this->state = this->newState(command);
  }
  return this->state == this->endState;
}

Automaton::State Automaton::newState(char in) {
  Automaton::State result = this->state;

  switch (this->state) {
    case Automaton::State::STATE_1:
      if (in == '1')
        result = Automaton::State::STATE_2;
      break;
    case Automaton::State::STATE_2:
      if (in == '0')
        result = Automaton::State::STATE_3;
      break;
    case Automaton::State::STATE_3:
      if (in == '0' || in == '1')
        result = Automaton::State::STATE_2;
      break;
  }

  return result;
}
