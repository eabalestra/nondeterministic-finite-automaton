#include <stdlib.h>
#include <stdio.h>
#include "nf-automata/nf-automata.h"

int main()
{
  NFA *nfa = create_nfa();
  int userChoice = 7;

  char filename[50];
  printf("Choose an automaton number from example-automatons : ");
  scanf("%d", &userChoice);
  sprintf(filename, "example-automatons/automata_%d.dot", userChoice);

  read_from_file(nfa, filename);
  int menu = 0;
  char input[100];

  while (menu != 5)
  {
    printf("\n1. Open NFA\n");
    printf("2. Check if a string is accepted by the NFA\n");
    printf("3. Transform NFA to DFA\n");
    printf("4. Choose another automaton\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &menu);
    switch (menu)
    {
    case 1:
      printf("Opening the NFA automata in xdot...\n");
      char command[50];
      sprintf(command, "xdot example-automatons/automata_%d.dot", userChoice);
      system(command);
      break;
    case 2:
      printf("Enter the string to check: ");
      scanf("%s", input);
      int accepted = belongs(nfa, input);
      if (accepted)
      {
        printf("\nThe input string is accepted by the NFA\n");
      }
      else
      {
        printf("\nThe input string is not accepted by the NFA\n");
      }
      break;
    case 3:
      DFA *dfa = nfa_to_dfa(nfa);
      dfa_to_dot(dfa, "example-automatons/automata_dfa.dot");
      printf("Opening the DFA automata in xdot...\n");

      system("xdot example-automatons/automata_dfa.dot");
      break;
    case 4:
      main();
      break;
    case 5:
      printf("Exiting the program.\n");
      exit(0);
    default:
      printf("Invalid choice. Please enter a number between 1 and 5.\n");
      break;
    }
  }
}