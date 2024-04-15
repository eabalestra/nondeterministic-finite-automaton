//
// Created by agustin on 09/04/24.
//

#ifndef NF_AUTOMATA_SET_H
#define NF_AUTOMATA_SET_H

#define MAX_SIZE 100

typedef struct
{
  int elements[MAX_SIZE];
  int size;
} Set;

Set *create_set();
void insert_set(Set *set, int element);
int contains(Set *set, int element);
void print_set(Set *set);

#endif // NF_AUTOMATA_SET_H
