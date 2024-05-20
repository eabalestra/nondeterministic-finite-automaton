#ifndef GRID_H
#define GRID_H
#define NUM_SETS 10

#include "../df-automata/df-automata.h"
#include "../sets/set.h"

typedef struct
{
  int number[DET_MAX_STATES][DET_MAX_SYMBOLS];
  int no_end_cant_states;
  int end_cant_states;
} Grid;

Grid *create_grid();
void calculate_grid(DFA *dfa, Grid *grid, Set *no_end_states[], Set *end_states[]);
void print_grid(Grid *grid, char alphabet[]);
Grid *copy_grid(Grid *grid);
void grid_clean(Grid *grid, Set *no_end_states[], Set *end_states[]);
int grid_equals(Grid *grid1, Grid *grid2);

void init_parts(DFA *dfa, Set *no_end_states[], Set *end_states[]);
void calculate_parts(DFA *dfa, Set *no_end_states[], Set *end_states[], Grid *grid);
void print_parts(Set *no_end_state[], Set *end_state[]);
int sum_cant_parts(Grid *grid, Set *no_end_state[], Set *end_state[]);

#endif // GRID_H
