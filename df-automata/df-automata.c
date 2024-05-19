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

// Implementacion de minimization, hay que factorizarlo y quitarle las cosas que estan de mas
// MUCHO COMENTARIO, SACARLOS ANTES DE ENTREGAR, PREGUNTENME SI NO ENTIENDEN ALGO:) ATTE:YO
DFA *minimization(DFA *dfa)
{
  Set *no_end_states[NUM_SETS]; // Arreglo de punteros a conjuntos
  Set *end_states[NUM_SETS];

  // aca hacemos la primera division entre Set de estados finales y los no finales
  init_parts(dfa, no_end_states, end_states);

  // creamos la tabla de transiciiones
  Grid *grid = create_grid();
  // necesito para los ciclos cuantos estados tiene el conjunto de finales y NO-finales, asiq los guarde en grid:)
  grid->no_end_cant_states = no_end_states[0]->size;
  grid->end_cant_states = end_states[0]->size;
  // Calcula a que clase de equivalencia va cada stado por cada letra
  calculate_grid(dfa, grid, no_end_states, end_states);
  // quitar esto antes de la entrega finar
  print_grid(grid);
  printf("-------------------------------------------------------------------------\n");
  // hago una copia de la grilla para poder comparar con la nueva que voy a generar y saber cuando para el ciclo y que
  // ya no puedo separar mas las equivalencias
  Grid *copy = create_grid();
  // condicion de finalizacion
  while (grid_equals(copy, grid) != 1)
  {
    // actualizo la copia
    copy = copy_grid(grid);
    // aca dentro del calculate, a partir de grid, separo las clases de equivalencia y vuelvo a calcular grid
    calculate_parts(dfa, no_end_states, end_states, grid);
  }
  // sacar esto pero es como quedan las particiones al finalizar el ciclo
  print_parts(no_end_states, end_states);

  // construir el automata en base a grid
  grid_clean(grid, no_end_states, end_states);
  print_grid(grid);
  int cant_parts = sum_cant_parts(grid, no_end_states, end_states);
  printf("Cantidad total de particiones: %d \n", cant_parts);
  DFA *result = create_dfa();
  printf("q%d->q%d [label=%c];\n", 0, grid->number[0][0], 'a');
  /**
  for (int i = 0; i < cant_parts; i++)
  {
    if (grid->number[i][0] != -1)
    {
      for (int j = 'a'; j <= 'z'; j++)
      {
        //esto no anda, hay que arreglar lo del malloc
        det_add_transition(result, i, grid->number[i][j-'a'], j);
      }
    }
  }
  print_dfa(result);*/
  print_parts(no_end_states, end_states);
  return result;
}
