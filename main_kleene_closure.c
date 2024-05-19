#include <stdlib.h>
#include <stdio.h>
#include "nf-automata/nf-automata.h"

int main()
{
  NFA *nfa = create_nfa();
  non_det_add_transition(nfa, 0, 1, LAMBDA_SYMBOL);
  non_det_add_transition(nfa, 0, 3, LAMBDA_SYMBOL);
  non_det_add_transition(nfa, 1, 2, 'a');
  non_det_add_transition(nfa, 3, 4, 'b');
  non_det_add_transition(nfa, 2, 5, LAMBDA_SYMBOL);
  non_det_add_transition(nfa, 4, 5, LAMBDA_SYMBOL);
  non_det_set_accepting(nfa, 5, 1);
  read_from_file(nfa, "example-automatons/automata_9.dot");

  nfa = kleene_closure(nfa);
  nfa_to_dot(nfa, "example-automatons/automata_dfa.dot");
  system("xdot example-automatons/automata_dfa.dot");
  DFA *dfa = nfa_to_dfa(nfa);
  dfa = nfa_to_dfa(nfa);
  dfa_to_dot(dfa, "example-automatons/automata_dfa.dot");
  system("xdot example-automatons/automata_dfa.dot");
}
