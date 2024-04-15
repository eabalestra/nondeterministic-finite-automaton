#include "nf-automata.h"
#include "../sets/set.h"
#include "../df-automata/df-automata.h"

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

NFA *create_nfa(char alphabet[])
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

  for (int i = 0; i < strlen(alphabet); i++)
  {
    nfa->alphabet[i] = alphabet[i];
  }

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
  State *q0 = create_state();
  add_to_state(q0, nfa->initial_state);
  det_add_state(dfa, lambda_closure(nfa, q0));

  for (int i = 0; i < dfa->states_cant; i++)
  {
    State *current_state = dfa->states[i];
    for (int symbol = 0; symbol < strlen(nfa->alphabet); symbol++)
    {
      State *pre_lambda_state = move(nfa, current_state, nfa->alphabet[symbol]);
      State *new_state = lambda_closure(nfa, pre_lambda_state);
      int last_index = det_add_state(dfa, new_state);
      if (new_state->enteros->size != 0)
      {
        det_add_transition(dfa, i, last_index, nfa->alphabet[symbol]);
      }
    }
  }
  set_accepting(dfa, nfa);
  return dfa;
}

State *lambda_closure(NFA *nfa, State *state)
{
  State *result = create_state();

  for (int i = 0; i < state->enteros->size; ++i)
  {
    add_to_state(result, state->enteros->elements[i]);
  }

  Node *node;

  for (int i = 0; i < (state->enteros->size); ++i)
  {
    int int_state = state->enteros->elements[i];
    node = nfa->transitions[int_state][NON_DET_MAX_SYMBOLS - 1];

    while (node != NULL && node->data != -1)
    {
      add_to_state(state, node->data);
      add_to_state(result, node->data);
      node = node->next;
    }
  }

  return result;
}

State *move(NFA *nfa, State *state, char symbol)
{
  State *result = create_state();
  Node *node;

  for (int i = 0; i < state->enteros->size; ++i)
  {
    int int_state = state->enteros->elements[i];
    node = nfa->transitions[int_state][symbol - 'a'];

    while (node != NULL && node->data != -1)
    {
      add_to_state(result, node->data);
      node = node->next;
    }
  }

  return result;
}

int belongs(NFA *nfa, char input[])
{
    DFA *dfa = create_dfa();
    dfa = nfa_to_dfa(nfa);

    int index_current_state = dfa->initial_state;
    for (int i = 0; i < strlen(input); ++i)
    {
        char symbol = input[i];
        State *next_state = det_transition(dfa, index_current_state, symbol);
        if (next_state == NULL || next_state->enteros->size == 0)
        {
            return 0;
        }

        index_current_state = find_state_index(dfa, next_state);
    }

    if (dfa->states[index_current_state]->is_accepting == 1)
    {
        return 1;
    }
    return 0;
}

int find_state_index(DFA *dfa, State *state) {
    if (dfa == NULL || state == NULL) {
        return -1;
    }

    for (int i = 0; i < dfa->states_cant; i++) {
        if (equals(dfa->states[i], state)) {
            return i;
        }
    }

    return -1;
}

int belongs_without_processing_nfa(NFA *nfa, char input[])
{
  return recursive_belongs_without_processing_nfa(nfa, nfa->initial_state, input);
}

int recursive_belongs_without_processing_nfa(NFA *nfa, int current_node, char input[])
{
  if (*input == '\0' && nfa->is_accepting[current_node])
  {
    return 1;
  }
  if (*input == '\0' && !nfa->is_accepting[current_node])
  {
    return 0;
  }
  if (current_node == ERROR_STATE)
  {
    return 0;
  }

  Node *next_node = non_det_transition(nfa, current_node, *input);
  while (next_node != NULL && next_node->data != ERROR_STATE)
  {
    char *input_copy = get_consumed_chain(nfa, current_node, input, next_node);
    if (recursive_belongs_without_processing_nfa(nfa, next_node->data, input_copy))
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
    return input;
  }
  int symbol_index = *input - 'a'; // Convert the symbol to an index
  if (nfa->transitions[current_node_data][symbol_index]->data != -1)
  {
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
