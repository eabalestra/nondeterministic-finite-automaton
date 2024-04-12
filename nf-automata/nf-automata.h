//
// Created by agustin and matias on 30/03/24.
//

#ifndef NF_AUTOMATA_NF_AUTOMATA_H
#define NF_AUTOMATA_NF_AUTOMATA_H

#include "../linked-lists/linked_list.h"
#include "../df-automata/df-automata.h"
#include "../sets/set.h"

#define NON_DET_MAX_STATES 100
#define NON_DET_MAX_SYMBOLS 28 // 27 letters + lambda
#define ERROR_STATE (-1)
#define LAMBDA_SYMBOL '_'

typedef struct
{
  Node *transitions[NON_DET_MAX_STATES][NON_DET_MAX_SYMBOLS];
  int is_accepting[NON_DET_MAX_STATES];
  int initial_state;
} NFA;

NFA *create_nfa();
void non_det_add_transition(NFA *nfa, int from, int to, char symbol);
void non_det_set_accepting(NFA *nfa, int state, int is_accepting);
Node *non_det_transition(NFA *nfa, int current_state, char symbol);

// Exercise 2
void read_from_file(NFA *nfa, const char *filename);

// Exercise 3
DFA *nfa_to_dfa(NFA *nfa);
Set *lambda_closure(NFA *nfa, Set *states);
Set *move(NFA *nfa, Set *states, char symbol);

// Exercise 4


//
int belongs_non_det(NFA *nfa, char input[]);
int belongs(NFA *nfa, int current_node, char input[]);
char *get_consumed_chain(NFA *nfa, int current_node_data, char *input, Node *node);

void print_nfa(NFA *nfa);

void nfa_to_dot(NFA *nfa, const char *filename);
#endif // NF_AUTOMATA_NF_AUTOMATA_H
