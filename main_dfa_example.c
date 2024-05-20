#include <stdlib.h>
#include <stdio.h>
#include "nf-automata/nf-automata.h"

int main()
{
  DFA *dfa = create_dfa();

  State *s0 = create_state();
  add_to_state(s0, 0);
  State *s1 = create_state();
  add_to_state(s1, 1);

  det_add_state(dfa, s0);
  det_add_state(dfa, s1);
  det_set_accepting(dfa, 1);

  det_add_transition(dfa, 0, 1, 'a');
  det_add_transition(dfa, 0, 0, 'b');
  det_add_transition(dfa, 1, 0, 'b');

  print_dfa(dfa);
  dfa_to_dot(dfa, "example-automatons/automata_dfa.dot");
  system("xdot example-automatons/automata_dfa.dot");
}