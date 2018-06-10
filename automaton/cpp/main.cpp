#include <vector>
#include <iostream>
#include "automaton/automaton.hpp"

using namespace std;

int main() {
  Automaton automata;
  cout << boolalpha
       << automata.read_commands({'1', '0', '0', '1'}) << endl
       << automata.read_commands({'1'}) << endl;
  return 0;
}
