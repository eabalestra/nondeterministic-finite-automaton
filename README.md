# Non-Deterministic Finite Automaton (NFA) to Deterministic Finite Automaton (DFA) Converter

This project is a C program that converts a Non-Deterministic Finite Automaton (NFA) to a Deterministic Finite Automaton (DFA). It also provides functionality to check if a given string is accepted by the NFA or the resulting DFA.

## Authors
- Balestra Edgar Agustin - 43765718
- Bernardi Quiroga Matias - 43808286
- Urquiza Carlos Alfredo - 44112441 

## Usage

The program provides a menu with the following options:

1. Open NFA: This option opens the NFA in xdot.
2. Check if a string is accepted by the NFA: This option prompts the user to enter a string and then checks if the string is accepted by the NFA.
3. Transform NFA to DFA: This option converts the NFA to a DFA and opens the DFA in xdot.
4. Choose another automaton: This option allows the user to select another NFA from the `example-automatons` directory.
5. Exit: This option exits the program.

## Code Structure

The code is divided into several files:

- `main.c`: This file contains the main function which provides the menu and handles user input.
- `nf-automata/nf-automata.c`: This file contains the implementation of the NFA and the conversion from NFA to DFA.
- `df-automata/df-automata.c`: This file contains the implementation of the DFA.

## Building and Running

To build the project, use the following command:

```bash
gcc -o main main.c nf-automata/nf-automata.c df-automata/df-automata.c sets/set.c linked-lists/linked_list.c state/state.c
```

To run the project, use the following command:

```bash
./main
```
