#include <stdlib.h>
#include <stdio.h>
#include "nf-automata/nf-automata.h"

int main()
{
  NFA *nfa = create_nfa();
  read_from_file(nfa, "example-automatons/automata_17.dot");

  DFA *dfa = nfa_to_dfa(nfa);
  dfa_to_dot(dfa, "example-automatons/automata_dfa.dot");
  system("xdot example-automatons/automata_dfa.dot");

  dfa = minimization(dfa);
  dfa_to_dot(dfa, "example-automatons/automata_dfa.dot");
  system("xdot example-automatons/automata_dfa.dot");
}