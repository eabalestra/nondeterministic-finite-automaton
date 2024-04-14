#include "df-automata.h"

#include <stdio.h>
#include <stdlib.h>

DFA *create_dfa()
{
  DFA *dfa = malloc(sizeof(DFA));
  for (int from = 0; from < DET_MAX_STATES; from++)
  {
    for (int symbol = 0; symbol < DET_MAX_SYMBOLS; symbol++)
    {
      dfa->transitions[from][symbol] = -1;
    }
  }
  dfa->initial_state = 0;
  dfa->states_cant = 0;
  return dfa;
}

int det_add_state(DFA *dfa, State *state)
{
  int last_index = dfa->states_cant;

  if (state->enteros->size == 0)
  {
    return -1;
  }

  for (int i = 0; i < last_index; i++)
  {
    if (equals(dfa->states[i], state))
    { // dont add it if exists
      return i;
    }
  }

  dfa->states[last_index] = state;
  dfa->states_cant = last_index + 1;
  return last_index;
}

void det_add_transition(DFA *dfa, int from, int to, char symbol)
{
  dfa->transitions[from][symbol - 'a'] = to;
}

void det_set_accepting(DFA *dfa, int index)
{
  State *state = dfa->states[index];
  state->is_accepting = 1;
}

State *det_transition(DFA *dfa, int current_state, char symbol)
{
  int symbol_index = symbol - 'a'; // Convert the symbol to an index
  int next_state_index = dfa->transitions[current_state][symbol_index];
  if (next_state_index == -1)
  {
    printf("No deterministic transition defined for state %d with symbol %c\n", current_state, symbol);
    return create_state(-1);
  }

  return dfa->states[next_state_index];
}