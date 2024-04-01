//
// Created by agustin on 30/03/24.
//

#ifndef NF_AUTOMATA_NF_AUTOMATA_H
#define NF_AUTOMATA_NF_AUTOMATA_H

#define MAX_STATES 100
#define MAX_SYMBOLS 27

typedef struct {
    int transitions[MAX_STATES][MAX_SYMBOLS];
    int is_accepting[MAX_STATES];
    int initial_state;
} NFA;

NFA* create_nfa();
void add_transition(NFA* nfa, int from, int to, char symbol);
void set_accepting(NFA* nfa, int state, int is_accepting);
int transition(NFA* nfa, int current_state, char symbol);

void read_from_file(NFA* nfa, const char* filename);

#endif //NF_AUTOMATA_NF_AUTOMATA_H
