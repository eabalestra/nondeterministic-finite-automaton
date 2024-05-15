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

  for (int i = 0; i < dfa->states_cant; i++)
  {
    State *state = dfa->states[i];
    if (state->is_accepting)
    {
      print_state(state);
      printf("IS ACCEPTING\n");
    }

    for (int symbol = 0; symbol < DET_MAX_SYMBOLS; symbol++)
    {
      State *goal = det_transition(dfa, i, 'a' + symbol);
      if (goal != NULL)
      {
        printf("-----------------------------------\nTransition from ");
        print_state(state);
        printf("by '%c' to ", 'a' + symbol);
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

void minimization(DFA *dfa){
  Set *states_inic[NUM_SETS]; // Arreglo de punteros a conjuntos
  Set *states_end[NUM_SETS];
  Set *states_x[NUM_SETS];

  // Inicializa cada elemento del arreglo
  for (int i = 0; i < NUM_SETS; i++) {
      states_inic[i] = create_set();
      states_end[i] = create_set();
      states_x[i] = create_set();
  }
  Set x, y;

  // División entre estados finales y no finales
  for (int  i = 0; i < dfa->states_cant ; i++) {
    if (!(dfa->states[i]->is_accepting == 1)) {
      insert_set(states_inic[0], i);
    } else {
      insert_set(states_end[1], i);
    }
  }
  print_set(states_inic[0]);
  print_set(states_end[1]);

  // Modificación de la grilla
  for (int i = 0; i < DET_MAX_STATES; i++) {
    for (int j = 0; j < DET_MAX_SYMBOLS; j++) {
      if (dfa->transitions[i][j] != -1) {
        //grid->cell[i][j] = 1; // Modifica la celda en la grilla
        return 0;
      }else{
        //grid->cell[i][j] = 0; // Modifica la celda en la grilla
        return 0;
      }
    }
  }
}