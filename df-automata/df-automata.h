#include "../sets/set.h"
#include "../state/state.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef NF_AUTOMATA_DF_AUTOMATA_H
#define NF_AUTOMATA_DF_AUTOMATA_H

#define DET_MAX_STATES 100
#define DET_MAX_SYMBOLS 27



typedef struct
{
  State *states[DET_MAX_STATES];
  int transitions[DET_MAX_STATES][DET_MAX_SYMBOLS];
  int initial_state;
  int states_cant;
} DFA;

DFA *create_dfa();
int det_add_state(DFA *dfa, State *state);
void det_add_transition(DFA *dfa, int from, int to, char symbol);
void det_set_accepting(DFA *dfa, int index);
State *det_transition(DFA *dfa, int current_state, char symbol);
void print_dfa(DFA *dfa);
void dfa_to_dot(DFA *dfa, const char *filename);

/**
 * TP 2
 */

typedef struct {
    int number[DET_MAX_STATES][DET_MAX_SYMBOLS];
    int no_end_cant_states;
    int end_cant_states;
} Grid;

Grid *create_grid();
void calculate_grid(DFA *dfa, Grid *grid, Set *no_end_states[],Set *end_states[]);
void print_grid(Grid *grid);
Grid *copy_grid(Grid *grid);
void grid_clean(Grid *grid, Set *no_end_states[], Set *end_states[]);


void init_parts(DFA *dfa, Set *no_end_states[], Set *end_states[]);
void calculate_parts(DFA *dfa, Set *no_end_states[], Set *end_states[], Grid *grid);
void print_parts(Set *no_end_state[], Set *end_state[]);
int sum_cant_parts(Grid *grid, Set *no_end_state[], Set *end_state[]);


//2
DFA *minimization(DFA *dfa);
#endif // NF_AUTOMATA_DF_AUTOMATA_H
