#include <stdio.h>
#include "nf-automata.h"

int main() {
    NFA* nfa = create_nfa();
    add_transition(nfa, 0, 1, 'a');
    add_transition(nfa, 1, 2, 'b');
    add_transition(nfa, 1, 1, 'b');
    add_transition(nfa, 1, 3, 'a');
    add_transition(nfa, 3, 3, 'a');
    add_transition(nfa, 3, 3, 'b');

    set_accepting(nfa, 2, 1);

    //read_from_file(nfa, "/home/agustin/Desktop/university/automatas-y-lenguajes/repository/nf-automata/automatas/automata.dot");

    int current_state = nfa->initial_state;
    char input[] = "abb";

    for (int i = 0; i < sizeof(input) - 1; i++) {
        current_state = transition(nfa, current_state, input[i]);
    }

    if (nfa->is_accepting[current_state]) {
        printf("The input string is accepted by the NFA\n");
    } else {
        printf("The input string is not accepted by the NFA\n");
    }

    return 0;
}