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
  Node *node = nfa->transitions[from][symbol - 'a'];
  if (node->data == -1)
  {
    node->data = to;
  }
  else
  {
    while (node->next != NULL)
    {
      node = node->next;
    }
    node->next = createNode(to);
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
  Node *initial = nfa->transitions[nfa->initial_state][input[0] - 'a'];
  input = input + 1;
  return belongs(nfa, initial, input);
}

int belongs(NFA *nfa, Node *current, char *input)
{

  if (current == NULL || current->data == ERROR_STATE)
  {
    printf("nopath !\n");
    return 0;
  }
  if (*input == '\0' && !nfa->is_accepting[current->data])
  {
    printf("string consumed totally and finished on an NON accepting state !\n");
    return 0;
  }
  if (*input == '\0' && nfa->is_accepting[current->data])
  {
    printf("string consumed totally and finished on an ACCEPTING state !\n");
    return 1;
  }

  while (current != NULL && current->data != ERROR_STATE)
  {
    printf("Next trans: from state: %d and string: %s\n", current->data, input);
    Node *node = transition(nfa, current->data, *input);
    if (belongs(nfa, node, input + 1))
    {
      return 1;
    }
    current = current->next;
  }

  return 0;
}

int belongs2(NFA *nfa, Node *current, char input[])
{
  if (current == NULL)
  {
    return 0;
  }
  int accepted;
  int current_state_in_int;

  for (int i = 0; i < strlen(input); i++)
  {
    current_state_in_int = current->data;
    current = transition(nfa, current_state_in_int, input[i]);
    input = input + 1;

    if (current == NULL || current->data == ERROR_STATE)
    {
      return 0;
    }

    if (current->next != NULL)
    {

      while (current->next != NULL)
      {
        accepted = belongs(nfa, current, input + 1);
        if (accepted)
        {
          return 1;
        }
        current = current->next;
      }

      return accepted;
    }
    if (current->next != NULL)
    {
      current = current->next;
    }
  }

  current_state_in_int = current->data;
  if (nfa->is_accepting[current_state_in_int])
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

  regex_t regex_transition, regex_accepting;
  regcomp(&regex_transition, "q([0-9]+)->q([0-9]+) \\[label=\"([a-z,_]+)\"\\];", REG_EXTENDED);
  regcomp(&regex_accepting, "q([0-9])\\[shape=doublecircle\\];", REG_EXTENDED);

  char line[256];
  while (fgets(line, sizeof(line), file))
  {
    regmatch_t groups[4];

    if (!regexec(&regex_accepting, line, 2, groups, 0))
    {
      int state = atoi(line + groups[1].rm_so);
      printf("Setting state %d as accepting\n", state);
      set_accepting(nfa, state, 1);
    }
    else if (!regexec(&regex_transition, line, 4, groups, 0))
    {
      int from = atoi(line + groups[1].rm_so);
      int to = atoi(line + groups[2].rm_so);
      char *symbols = strndup(line + groups[3].rm_so, groups[3].rm_eo - groups[3].rm_so);
      char *symbol = strtok(symbols, ",");
      while (symbol != NULL)
      {
        printf("Adding transition from %d to %d with symbol %s\n", from, to, symbol);
        add_transition(nfa, from, to, *symbol);
        symbol = strtok(NULL, ",");
      }
      free(symbols);
    }
  }

  regfree(&regex_transition);
  regfree(&regex_accepting);
  fclose(file);
}

char *substring(char *destination, const char *source, int beg, int n)
{
  strncpy(destination, (source + beg), n);
  return destination;
}

void print_nfa(NFA *nfa)
{
  printf("Initial state: q%d\n", nfa->initial_state);

  for (int from = 0; from < MAX_STATES; from++)
  {
    for (int symbol = 0; symbol < MAX_SYMBOLS; symbol++)
    {
      Node *node = nfa->transitions[from][symbol];
      while (node != NULL && node->data != -1)
      {
        printf("Transition: q%d --%c--> q%d\n", from, 'a' + symbol, node->data);
        node = node->next;
      }
    }
    if (nfa->is_accepting[from])
    {
      printf("State q%d is an accepting state\n", from);
    }
  }
}