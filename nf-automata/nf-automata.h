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
  char alphabet[NON_DET_MAX_SYMBOLS];
} NFA;

NFA *create_nfa();
void non_det_add_transition(NFA *nfa, int from, int to, char symbol);
void non_det_set_accepting(NFA *nfa, int state, int is_accepting);
Node *non_det_transition(NFA *nfa, int current_state, char symbol);
void print_alphabet(NFA *nfa);
// Exercise 2
void read_from_file(NFA *nfa, const char *filename);

// Exercise 3
DFA *nfa_to_dfa(NFA *nfa);
State *lambda_closure(NFA *nfa, State *state);
State *move(NFA *nfa, State *state, char symbol);

// Exercise 4
int belongs(NFA *nfa, char input[]);
int find_state_index(DFA *dfa, State *next_state);

// belongs que no transforma el NFA a DFA, no acepta trasiciones lambda en bucle
int belongs_without_processing_nfa(NFA *nfa, char input[]);
int recursive_belongs_without_processing_nfa(NFA *nfa, int current_node, char input[]);
char *get_consumed_chain(NFA *nfa, int current_node_data, char *input, Node *node);

void print_nfa(NFA *nfa);
void set_accepting(DFA *dfa, NFA *nfa);

void nfa_to_dot(NFA *nfa, const char *filename);

/**
 * TP 2
 */
int num_of_states(NFA *nfa);

// Exercise 1
// (a)
NFA *nfa_union(NFA *nfa1, NFA *nfa2);

// (b)
NFA *concatenation(NFA *nfaA, NFA *nfa2);

// (c)
NFA *kleene_closure(NFA *nfa);

void copy_automata(NFA *nfa, NFA *result);
void copy_automata_with_index(NFA *nfa, NFA *result, int index);

#endif // NF_AUTOMATA_NF_AUTOMATA_H
