#include <stdio.h>
#include "nf-automata.h"

int main()
{
  // create
  NFA *nfa = create_nfa();

  // AUTOMATA PANCHO
  /*add_transition(nfa, 0, 1, 'a');
  add_transition(nfa, 0, 2, 'a');
  add_transition(nfa, 0, 0, 'b');
  add_transition(nfa, 1, 2, 'b');
  add_transition(nfa, 2, 2, 'b');
  add_transition(nfa, 1, 3, 'a');
  add_transition(nfa, 3, 3, 'a');
  add_transition(nfa, 3, 3, 'b');
   */

  // AUTOMATA MATI PARA CHECKEAR ROLLBACK
  /*add_transition(nfa, 0, 1, 'a');
  add_transition(nfa, 1, 1, 'a');
  add_transition(nfa, 1, 1, 'b');
  add_transition(nfa, 1, 4, 'a');
  add_transition(nfa, 1, 3, 'a');
  add_transition(nfa, 0, 2, 'a');
  add_transition(nfa, 2, 3, 'b');
  add_transition(nfa, 3, 3, 'a');
  add_transition(nfa, 3, 3, 'b');
  set_accepting(nfa, 3, 1);*/

  add_transition(nfa, 0, 1, 'b');
  add_transition(nfa, 0, 2, '_');
  add_transition(nfa, 0, 2, 'b');
  add_transition(nfa, 1, 1, 'a');
  add_transition(nfa, 1, 2, 'a');
  add_transition(nfa, 2, 3, '_');
  add_transition(nfa, 3, 4, '_');
  add_transition(nfa, 4, 2, 'a');
  set_accepting(nfa, 1, 1);
 

/*   add_transition(nfa, 0, 0, '_');
  add_transition(nfa, 0, 1, 'a');
  add_transition(nfa, 1, 1, '_');
  add_transition(nfa, 1, 0, '_'); */
  // read_from_file(nfa, "/home/agustin/Desktop/university/automatas-y-lenguajes/repository/nondeterministic-finite-automaton/automatas/automata_4.dot");

  // print automata
  printf("\n");
  print_nfa(nfa);

  // check if a string is accepted
  char input[] = "ba";
  printf("\nInput string: %s\n", input);
  int accepted = belongs_non_det(nfa, input);

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