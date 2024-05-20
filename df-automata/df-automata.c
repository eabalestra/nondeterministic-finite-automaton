#include "df-automata.h"

#include <stdio.h>
#include <stdlib.h>
#include "../nf-automata/nf-automata.h"

#define NUM_SETS 10
#define true 1
#define false 0

DFA *create_dfa()
{
  DFA *dfa = malloc(sizeof(DFA));
  for (int from = 0; from < DET_MAX_STATES; from++)
  {
    for (int symbol = 0; symbol < DET_MAX_SYMBOLS; symbol++)
    {
      dfa->transitions[from][symbol] = -1;
      dfa->alphabet[symbol] = -1;
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
  add_letter_alphabet(dfa, symbol);
}

void add_letter_alphabet(DFA *dfa, char letter)
{
  int cond = 1;
  int i = 0;
  while (i < DET_MAX_SYMBOLS && dfa->alphabet[i] != -1)
  {
    if (letter == dfa->alphabet[i])
    {
      cond = 0;
    }
    i++;
  }
  if (cond == 1)
  {
    dfa->alphabet[i] = letter;
  }
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
    return NULL;
  }

  return dfa->states[next_state_index];
}

void set_accepting(DFA *dfa, NFA *nfa)
{
  for (int i = 0; i < dfa->states_cant; i++)
  {
    for (int j = 0; j < DET_MAX_SYMBOLS; j++)
    {
      if (nfa->is_accepting[j] && contains(dfa->states[i]->enteros, j))
      {
        det_set_accepting(dfa, i);
      }
    }
  }
}

void print_dfa(DFA *dfa)
{
  printf("DFA STATES: \n");
  for (int i = 0; i < dfa->states_cant; i++)
  {
    State *state = dfa->states[i];
    print_state(state);
    if (state->is_accepting == 1)
    {
      printf("Is an accepting state\n");
    }
  }

  for (int i = 0; i < dfa->states_cant; i++)
  {
    printf("-----------------------------------\nTransitions from: \n");
    State *state = dfa->states[i];
    print_state(state);

    for (int symbol = 0; symbol < DET_MAX_SYMBOLS; symbol++)
    {
      State *goal = det_transition(dfa, i, 'a' + symbol);
      if (goal != NULL)
      {
        printf("'%c' -> ", 'a' + symbol);
        print_state(goal);
      }
    }
  }
}

void dfa_to_dot(DFA *dfa, const char *filename)
{
  FILE *file = fopen(filename, "w");

  if (file == NULL)
  {
    printf("Error creating dot file: %s\n", filename);
    return;
  }

  fprintf(file, "digraph{\n");
  fprintf(file, "    rankdir=LR;\n");
  fprintf(file, "    inic[shape=point];\n");
  fprintf(file, "\n    inic->q%i;\n\n", dfa->initial_state);

  for (int from = 0; from < dfa->states_cant; from++)
  {
    for (int symbol = 0; symbol < DET_MAX_SYMBOLS; symbol++)
    {
      int data = dfa->transitions[from][symbol];
      if (data != -1)
      {
        fprintf(file, "    q%d->q%d [label=\"%c\"];\n", from, data, 'a' + symbol);
      }
    }
    if (dfa->states[from]->is_accepting == 1)
    {
      fprintf(file, "\n    q%d[shape=doublecircle];\n", from);
    }
  }
  fprintf(file, "}");
  for (int state = 0; state < dfa->states_cant; state++)
  {
    fprintf(file, "\n//State q%i: {", state);
    for (int i = 0; i < dfa->states[state]->enteros->size; i++)
    {
      fprintf(file, "%d", dfa->states[state]->enteros->elements[i]);
      if (i < dfa->states[state]->enteros->size - 1)
      {
        fprintf(file, ", ");
      }
    }
    fprintf(file, "}\n");
  }

  fclose(file);
}

void dfa_print_alphabet(DFA *dfa)
{
  printf("Alphabet: ");
  for (int i = 0; dfa->alphabet[i] != -1; i++)
  {
    printf("%c ", dfa->alphabet[i]);
  }
  printf("\n");
}

DFA *minimization(DFA *dfa)
{
  Set *no_end_states[dfa->states_cant - 1];
  Set *end_states[dfa->states_cant - 1];

  // Inicializa las particiones de estados finales y no finales
  init_parts(dfa, no_end_states, end_states);

  Grid *grid = create_grid();
  grid->no_end_cant_states = no_end_states[0]->size;
  grid->end_cant_states = end_states[0]->size;

  calculate_grid(dfa, grid, no_end_states, end_states); // Calcula a que clase de equivalencia va cada estado por cada letra
  print_grid(grid, dfa->alphabet);
  Grid *copy = create_grid();
  while (grid_equals(copy, grid) != 1)
  {
    copy = copy_grid(grid);
    // Calcula nuevas particiones y vuelve a calcular el grid
    calculate_parts(dfa, no_end_states, end_states, grid);
  }

  // Construir el automata en base al grid
  grid_clean(grid, no_end_states, end_states);
  int cant_parts = sum_cant_parts(grid, no_end_states, end_states);
  DFA *min_dfa = create_dfa();
  print_grid(grid, dfa->alphabet);

  for (int i = 0; i < cant_parts; i++)
  {
    if (grid->number[i][0] != -2) // Crea los estados validos
    {
      State *s = create_state();
      add_to_state(s, i);
      int new_state_index = det_add_state(min_dfa, s);

      if (dfa->states[i]->is_accepting == 1)
      {
        det_set_accepting(min_dfa, new_state_index);
      }

      for (int j = 0; j < DET_MAX_SYMBOLS; j++)
      {
        if (grid->number[i][j] != -1) // Crea las tansiciones validas
        {
          char symbol = 'a' + j;
          int to_state = grid->number[i][j];
          det_add_transition(min_dfa, new_state_index, to_state, symbol);
        }
      }
    }
  }
  print_parts(no_end_states, end_states);
  return min_dfa;
}
