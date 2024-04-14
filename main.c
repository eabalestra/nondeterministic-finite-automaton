#include <stdio.h>
#include "nf-automata/nf-automata.h"
#include "sets/state.h"

int main()
{

  char alphabet[3] = {'a', 'b'};
  NFA *nfa = create_nfa(alphabet);

  // read_from_file(nfa, "/home/agustin/Desktop/university/automatas-y-lenguajes/repository/nondeterministic-finite-automaton/example-automatons/automata_6.dot");
  /* read_from_file(nfa, "/home/carlos/Escritorio/Uni/AutomatasYLenguajes/tp1m/nondeterministic-finite-automaton/example-automatons/automata_7.dot"); */

  read_from_file(nfa, "/home/matybq/UNRC/automatas/nondeterministic-finite-automaton/example-automatons/automata_5.dot");
  printf("\n");
  // print_nfa(nfa);
  DFA *dfa = nfa_to_dfa(nfa);
  //dfa_to_dot(dfa, "/home/matybq/UNRC/automatas/nondeterministic-finite-automaton/example-automatons/automata_dfa.dot/home/matybq/UNRC/automatas/nondeterministic-finite-automaton/example-automatons/automata_dfa.dot");
  print_dfa(dfa);

  //  check if a string is accepted
  /*char input[] = "ba";
  printf("\nInput string: %s\n", input);
  int accepted = belongs_non_det(nfa, input);

  if (accepted)
  {
    printf("The input string is accepted by the NFA\n");
  }
  else
  {
    printf("The input string is not accepted by the NFA\n");
  }*/

  return 0;
}