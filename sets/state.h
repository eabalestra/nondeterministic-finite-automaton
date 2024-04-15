#ifndef STATE_H
#define STATE_H

#include "set.h"

typedef struct
{
  Set *enteros;
  int is_accepting;
} State;

State *create_state();
void add_to_state(State *state, int elem);
int equals(State *state1, State *state2);
void print_state(State *state);

#endif /* STATE_H */
