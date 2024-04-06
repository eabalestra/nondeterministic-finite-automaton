//
// Created by agustin and matias on 30/03/24.
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
  Node *node;
  if (symbol == LAMBDA_SYMBOL)
  {
      node = nfa->transitions[from][MAX_SYMBOLS - 1]; // the last index for lambda transition
  }
  else
  {
      node = nfa->transitions[from][symbol - 'a'];
  }

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
    Node *next_state;
    if (nfa->transitions[current_state][MAX_SYMBOLS - 1]->data != -1)
    {
        next_state = nfa->transitions[current_state][MAX_SYMBOLS - 1];
    }
    else
    {
        int symbol_index = symbol - 'a'; // Convert the symbol to an index
        next_state = nfa->transitions[current_state][symbol_index];
    }
    return next_state;
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

DFA nfa_to_dfa(NFA *nfa)
{
  DFA result;
  return result;
}

int belongs_non_det(NFA *nfa, char input[])
{
    Node *initial = transition(nfa, nfa->initial_state, *input);
    input = get_consumed_chain(nfa, nfa->initial_state, input);
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
    input = get_consumed_chain(nfa, current->data, input);
    if (belongs(nfa, node, input))
    {
      return 1;
    }
    current = current->next;
  }

  return 0;
}

char *get_consumed_chain(NFA *nfa, int current_node_data, char *input) {
    if (nfa->transitions[current_node_data][MAX_SYMBOLS - 1]->data != -1)
    {
        return input;
    }
    return input + 1;
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
