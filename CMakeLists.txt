cmake_minimum_required(VERSION 3.27)
project(nf_automata C)

set(CMAKE_C_STANDARD 17)

add_executable(nf_automata main.c
        nf-automata.c
        nf-automata.h)
