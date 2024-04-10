//
// Created by agustin on 09/04/24.
//

#ifndef NF_AUTOMATA_DF_AUTOMATA_H
#define NF_AUTOMATA_DF_AUTOMATA_H

#define DET_MAX_STATES 100
#define DET_MAX_SYMBOLS 27

typedef struct {
    int transitions[DET_MAX_STATES][DET_MAX_SYMBOLS];
    int is_accepting[DET_MAX_STATES];
    int initial_state;
} DFA;

DFA* create_dfa();
void det_add_transition(DFA* dfa, int from, int to, char symbol);
void det_set_accepting(DFA* dfa, int state, int is_accepting);
int det_transition(DFA* dfa, int current_state, char symbol);

#endif //NF_AUTOMATA_DF_AUTOMATA_H
