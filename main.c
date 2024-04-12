#include <stdio.h>
#include "nf-automata/nf-automata.h"

int main()
{
  // create
  NFA *nfa = create_nfa();

  read_from_file(nfa, "/home/carlos/Escritorio/Uni/AutomatasYLenguajes/Tp1/nondeterministic-finite-automaton/example-automatons/automata_3.dot");
  // print automata
  printf("\n");
  print_nfa(nfa);
  nfa_to_dot(nfa, "/home/carlos/Escritorio/Uni/AutomatasYLenguajes/Tp1/nondeterministic-finite-automaton/example-automatons/automata_6.dot");

  Set *initial_set = create_set();
  insert_set(initial_set, 0);
  initial_set = lambda_closure(nfa, initial_set);
  initial_set = lambda_closure(nfa, move(nfa, initial_set, 'a'));
  
  print_set(initial_set);
  

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