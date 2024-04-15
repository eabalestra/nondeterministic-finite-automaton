cmake_minimum_required(VERSION 3.27)
project(nf_automata C)

set(CMAKE_C_STANDARD 17)

add_executable(nf_automata main.c
        nf-automata/nf-automata.c
        nf-automata/nf-automata.h
        linked-lists/linked_list.c
        sets/set.c
        sets/set.h
        df-automata/df-automata.c
        df-automata/df-automata.h
        state/state.c
        state/state.h
)