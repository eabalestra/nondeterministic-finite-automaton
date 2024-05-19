#include <stdlib.h>
#include <stdio.h>
#include "nf-automata/nf-automata.h"

int main()
{
  NFA *nfa1 = create_nfa();
  NFA *nfa2 = create_nfa();
  NFA *nfa3 = create_nfa();
  NFA *nfaU = create_nfa();

  read_from_file(nfa1, "example-automatons/automata_14.dot");
  read_from_file(nfa2, "example-automatons/automata_15.dot");
  read_from_file(nfa3, "example-automatons/automata_16.dot");

  system("xdot example-automatons/automata_14.dot");
  system("xdot example-automatons/automata_15.dot");
  system("xdot example-automatons/automata_16.dot");

  nfaU = nfa_union(nfa1, nfa2);
  nfa_to_dot(nfaU, "example-automatons/automata_dfa.dot");
  system("xdot example-automatons/automata_dfa.dot");

  nfaU = concatenation(nfaU, nfa3);
  nfa_to_dot(nfaU, "example-automatons/automata_dfa.dot");
  system("xdot example-automatons/automata_dfa.dot");

  nfaU = kleene_closure(nfaU);
  nfa_to_dot(nfaU, "example-automatons/automata_dfa.dot");
  system("xdot example-automatons/automata_dfa.dot");

  DFA *dfa = create_dfa();
  dfa = nfa_to_dfa(nfaU);
  dfa_to_dot(dfa, "example-automatons/automata_dfa.dot");
  system("xdot example-automatons/automata_dfa.dot");

  dfa = minimization(dfa);
  print_dfa(dfa);
  dfa_to_dot(dfa, "example-automatons/automata_dfa.dot");
  system("xdot example-automatons/automata_dfa.dot");
}
