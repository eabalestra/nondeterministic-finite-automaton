//
// Created by agustin and matias on 30/03/24.
//
#include "nf-automata.h"
#include "../sets/set.h"
#include "../df-automata/df-automata.h"

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

NFA *create_nfa()
{
  NFA *nfa = malloc(sizeof(NFA));
  for (int from = 0; from < NON_DET_MAX_STATES; from++)
  {
    for (int symbol = 0; symbol < NON_DET_MAX_SYMBOLS; symbol++)
    {
      nfa->transitions[from][symbol] = createNode(-1);
    }
    nfa->is_accepting[from] = 0;
  }
  nfa->initial_state = 0;
  return nfa;
}

void non_det_add_transition(NFA *nfa, int from, int to, char symbol)
{
  Node *node;
  if (symbol == LAMBDA_SYMBOL)
  {
    node = nfa->transitions[from][NON_DET_MAX_SYMBOLS - 1]; // the last index for lambda non_det_transition
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

void non_det_set_accepting(NFA *nfa, int state, int is_accepting)
{
  nfa->is_accepting[state] = is_accepting;
}

Node *non_det_transition(NFA *nfa, int current_state, char symbol)
{
  int symbol_index = symbol - 'a'; // Convert the symbol to an index
  Node *next_state = nfa->transitions[current_state][symbol_index];

  if (nfa->transitions[current_state][NON_DET_MAX_SYMBOLS - 1]->data != ERROR_STATE)
  {
    insertAtEnd(&next_state, nfa->transitions[current_state][NON_DET_MAX_SYMBOLS - 1]->data);
  }

  return next_state;
}

DFA *nfa_to_dfa(NFA *nfa)
{
  DFA *dfa = create_dfa();
  Set *T = create_set();

  return dfa;
}

Set *lambda_closure(NFA *nfa, Set *states)
{
    Node *node;

    for (int i = 0; i < states->size; ++i)
    {
        int state = states->elements[i];
        node = nfa->transitions[state][NON_DET_MAX_SYMBOLS - 1];

        while (node != NULL && node->data != -1)
        {
            insert_set(states, node->data);
            node = node->next;
        }
    }

    return states;
}

Set *move(NFA *nfa, Set *states, char symbol)
{
    Set* result = create_set();
    Node *node;

    for (int i = 0; i < states->size; ++i)
    {
        int state = states->elements[i];
        node = nfa->transitions[state][symbol - 'a'];

        while (node != NULL && node->data != -1)
        {
            insert_set(states, node->data);
            insert_set(result, node->data);
            node = node->next;
        }
    }

    return result;
}

int belongs_non_det(NFA *nfa, char input[])
{
  return belongs(nfa, nfa->initial_state, input);
}

int belongs(NFA *nfa, int current_node, char *input)
{
  printf("current %d\n", current_node);
  if (*input == '\0' && nfa->is_accepting[current_node])
  {
    printf("string consumed totally and finished on an ACCEPTING state !\n");
    return 1;
  }
  if (*input == '\0' && !nfa->is_accepting[current_node])
  {
    printf("string consumed totally and finished on an NON accepting state !\n");
    return 0;
  }
  if (current_node == ERROR_STATE)
  {
    printf("nopath !\n");
    return 0;
  }

  Node *next_node = non_det_transition(nfa, current_node, *input);
  printList(next_node);
  while (next_node != NULL && next_node->data != ERROR_STATE)
  {
    char *input_copy = get_consumed_chain(nfa, current_node, input, next_node);
    printf("Next trans: from state: %d and string: %s\n", next_node->data, input_copy);
    if (belongs(nfa, next_node->data, input_copy))
    {
      return 1;
    }
    next_node = next_node->next;
  }

  return 0;
}

char *get_consumed_chain(NFA *nfa, int current_node_data, char *input, Node *node)
{
  if (nfa->transitions[current_node_data][NON_DET_MAX_SYMBOLS - 1]->data == node->data)
  {
    printf("Lambda from %d to %d\n", current_node_data, node->data);

    return input;
  }
  int symbol_index = *input - 'a'; // Convert the symbol to an index
  if (nfa->transitions[current_node_data][symbol_index]->data != -1)
  {
    printf("must consume from %d \n", current_node_data);
    return input + 1;
  }

  return NULL;
}

void print_nfa(NFA *nfa)
{
  printf("Initial state: q%d\n", nfa->initial_state);

  for (int from = 0; from < NON_DET_MAX_STATES; from++)
  {
    for (int symbol = 0; symbol < NON_DET_MAX_SYMBOLS; symbol++)
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
        non_det_set_accepting(nfa, state, 1);
    }
    else if (!regexec(&regex_transition, line, 4, groups, 0))
    {
      int from = atoi(line + groups[1].rm_so);
      int to = atoi(line + groups[2].rm_so);
      char *symbols = strndup(line + groups[3].rm_so, groups[3].rm_eo - groups[3].rm_so);
      char *symbol = strtok(symbols, ",");
      while (symbol != NULL)
      {
        printf("Adding non_det_transition from %d to %d with symbol %s\n", from, to, symbol);
          non_det_add_transition(nfa, from, to, *symbol);
        symbol = strtok(NULL, ",");
      }
      free(symbols);
    }
  }

  regfree(&regex_transition);
  regfree(&regex_accepting);
  fclose(file);
}

void nfa_to_dot(NFA *nfa, const char *filename) {
  FILE *file = fopen(filename, "w");

  if (file == NULL) {
    printf("Error creating dot file: %s\n", filename);
    return;
  }

  fprintf(file, "digraph{\n");
  fprintf(file, "    rankdir=LR;\n");
  fprintf(file, "    inic[shape=point];\n");
  fprintf(file, "\n    inic->d%i;\n\n", nfa->initial_state);


  for (int from = 0; from < NON_DET_MAX_STATES; from++)
  {
    for (int symbol = 0; symbol < NON_DET_MAX_SYMBOLS; symbol++)
    {
      Node *node = nfa->transitions[from][symbol];
      while (node != NULL && node->data != -1)
      {
        fprintf(file, "    q%d->q%d [label='%c']\n", from,node->data, 'a' + symbol);
        node = node->next;
      }
    }
    if (nfa->is_accepting[from])
    {
      fprintf(file, "\n    q%d[shape=doublecircle]\n", from);
    }
  }
  fprintf(file,"}");
}