//
// Created by agustin on 09/04/24.
//

#include "df-automata.h"

#include <stdio.h>
#include <stdlib.h>

DFA* create_dfa() {
    DFA* dfa = malloc(sizeof(DFA));
    for (int from = 0; from < DET_MAX_STATES; from++) {
        for (int symbol = 0; symbol < DET_MAX_SYMBOLS; symbol++) {
            dfa->transitions[from][symbol] = -1;
        }
        dfa->is_accepting[from] = 0;
    }
    dfa->initial_state = 0;
    return dfa;
}

void det_add_transition(DFA* dfa, int from, int to, char symbol) {
    dfa->transitions[from][symbol - 'a'] = to;
}

void det_set_accepting(DFA* dfa, int state, int is_accepting) {
    dfa->is_accepting[state] = is_accepting;
}

int det_transition(DFA* dfa, int current_state, char symbol) {
    int symbol_index = symbol - 'a'; // Convert the symbol to an index
    int next_state = dfa->transitions[current_state][symbol_index];
    if (next_state == -1) {
        printf("No deterministic transition defined for state %d with symbol %c\n", current_state, symbol);
        return current_state;
    }
    return next_state;
}
