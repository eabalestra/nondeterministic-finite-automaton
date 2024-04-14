#include <stdio.h>
#include "nf-automata/nf-automata.h"
#include "sets/state.h"

int main()
{
  // create
  char alphabet[3] = {'a', 'b', 'c'};

  NFA *nfa = create_nfa(alphabet);

  read_from_file(nfa, "/home/agustin/Desktop/university/automatas-y-lenguajes/repository/nondeterministic-finite-automaton/example-automatons/automata_6.dot");


  //read_from_file(nfa, "/home/matybq/UNRC/automatas/nondeterministic-finite-0automaton/example-automatons/automata_5.dot");
  printf("\n");
  //print_nfa(nfa);
  DFA *dfa = nfa_to_dfa(nfa);

  // check if a string is accepted
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