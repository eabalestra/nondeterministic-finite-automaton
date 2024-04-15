#include "../sets/set.h"
#include "../sets/state.h"

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

#endif // NF_AUTOMATA_DF_AUTOMATA_H
