//
// Created by agustin and matias on 30/03/24.
//

#ifndef NF_AUTOMATA_NF_AUTOMATA_H
#define NF_AUTOMATA_NF_AUTOMATA_H

#include "linked_list.h"

#define MAX_STATES 100
#define MAX_SYMBOLS 28 // 27 letters + lambda
#define ERROR_STATE (-1)
#define LAMBDA_SYMBOL '_'

typedef struct
{
  Node *transitions[MAX_STATES][MAX_SYMBOLS];
  int is_accepting[MAX_STATES];
  int initial_state;
} NFA;

typedef struct
{
  int transitions[MAX_STATES][MAX_SYMBOLS];
  int is_accepting[MAX_STATES];
  int initial_state;
} DFA;

NFA *create_nfa();
void add_transition(NFA *nfa, int from, int to, char symbol);
void set_accepting(NFA *nfa, int state, int is_accepting);
Node *transition(NFA *nfa, int current_state, char symbol);

// Exercise 2
void read_from_file(NFA *nfa, const char *filename);

// Exercise 3
DFA nfa_to_dfa(NFA *nfa);

// Exercise 4
int belongs_non_det(NFA *nfa, char input[]);
int belongs(NFA *nfa, Node *current, char input[]);
char *get_consumed_chain(NFA *nfa, int current_node_data, char *input);

void print_nfa(NFA *nfa);

#endif // NF_AUTOMATA_NF_AUTOMATA_H
