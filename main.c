#include <stdio.h>
#include "nf-automata.h"

int main()
{
  NFA *nfa = create_nfa();

  // AUTOMATA PANCHO
  
/*     add_transition(nfa, 0, 1, 'a');
    add_transition(nfa, 0, 2, 'a');
    add_transition(nfa, 0, 0, 'b');
    add_transition(nfa, 1, 2, 'b');
    add_transition(nfa, 2, 2, 'b');
    add_transition(nfa, 1, 3, 'a');
    add_transition(nfa, 3, 3, 'a');
    add_transition(nfa, 3, 3, 'b');
   */

  // AUTOMATA MATI PARA CHECKEAR ROLLBACK
  add_transition(nfa, 0, 1, 'a');
  add_transition(nfa, 1, 1, 'a');
  add_transition(nfa, 1, 1, 'b');
  add_transition(nfa, 1, 4, 'a');
  add_transition(nfa, 1, 3, 'a');
  add_transition(nfa, 0, 2, 'a');
  add_transition(nfa, 2, 3, 'b');
  add_transition(nfa, 3, 3, 'a');
  add_transition(nfa, 3, 3, 'b');
  set_accepting(nfa, 3, 1);

  // read_from_file(nfa, "/home/agustin/Desktop/university/automatas-y-lenguajes/repository/nondeterministic-finite-automaton/automatas/automata.dot");

  printf("\n");
  print_nfa(nfa);
  char input[] = "aaaabbbabba";
  printf("\nInput string: %s\n", input);
  int accepted = belongs_nondet(nfa, input);
  if (accepted)
  {
    printf("The input string is accepted by the NFA\n");
  }
  else
  {
    printf("The input string is not accepted by the NFA\n");
  }

  return 0;
}