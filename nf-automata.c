//
// Created by agustin on 30/03/24.
//

#include "nf-automata.h"
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

NFA *create_nfa()
{
  NFA *nfa = malloc(sizeof(NFA));
  for (int from = 0; from < MAX_STATES; from++)
  {
    for (int symbol = 0; symbol < MAX_SYMBOLS; symbol++)
    {
      nfa->transitions[from][symbol] = createNode(-1);
    }
    nfa->is_accepting[from] = 0;
  }
  nfa->initial_state = 0;
  return nfa;
}

void add_transition(NFA *nfa, int from, int to, char symbol)
{
  printf("Transition from %d to %d with symbol %c\n", from, to, symbol);
  Node *node = nfa->transitions[from][symbol - 'a'];
  if (node->data == -1)
  {
    nfa->transitions[from][symbol - 'a'] = createNode(to);
  }
  else
  {
    insertAtBeginning(&node, to);
  }
}

void set_accepting(NFA *nfa, int state, int is_accepting)
{
  nfa->is_accepting[state] = is_accepting;
}

Node *transition(NFA *nfa, int current_state, char symbol)
{
  int symbol_index = symbol - 'a'; // Convert the symbol to an index
  Node *next_state = nfa->transitions[current_state][symbol_index];
  return next_state;
}

int belongs_nondet(NFA *nfa, char input[])
{
  Node *initial = nfa->transitions[0][input[0]];
  return belongs(nfa, initial, input);
}

int belongs(NFA *nfa, Node *current, char input[])
{
  int accepted;
  int state;

  for (int i = 0; i < strlen(input); i++)
  {
    int state = current->data;
    current = transition(nfa, state, input[0]);
    if (current->data == ERROR_STATE)
    {
      return 0;
    }

    if (current->next != NULL)
    {

      while (current->next != NULL)
      {
        accepted = belongs(nfa, current, 'substring');
        if (accepted)
        {
          return 1;
        }
        current = current->next;
      }

      return accepted;
    }

    current = current->next;
  }

  state = current->data;
  if (nfa->is_accepting[state])
  {
    return 1;
  }
  return 0;
}

void read_from_file(NFA *nfa, const char *filename)
{
  FILE *file = fopen(filename, "r");
  if (file == NULL)
  {
    printf("Could not open file %s\n", filename);
    return;
  }

  regex_t regex_transition, regex_transition_2, regex_accepting;
  regcomp(&regex_transition, "q([0-9]+)->q([0-9]+) \\[label=\"([a-z,]+)\"\\];", REG_EXTENDED);
  regcomp(&regex_accepting, "q([0-9])\\[shape=doublecircle\\];", REG_EXTENDED);
  regcomp(&regex_transition_2, "q([0-9]+)->q([0-9]+) \\[label=\"([a-z,]+)\"\\];", REG_EXTENDED);

  char line[256];
  while (fgets(line, sizeof(line), file))
  {
    regmatch_t groups[4];

    if (!regexec(&regex_transition, line, 4, groups, 0))
    {
      int from = atoi(line + groups[1].rm_so);
      int to = atoi(line + groups[2].rm_so);
      char symbol = *(line + groups[3].rm_so);
      add_transition(nfa, from, to, symbol);
    }
    else if (!regexec(&regex_accepting, line, 2, groups, 0))
    {
      int state = atoi(line + groups[1].rm_so);
      set_accepting(nfa, state, 1);
    }
  }

  regfree(&regex_transition);
  regfree(&regex_accepting);
  fclose(file);
}