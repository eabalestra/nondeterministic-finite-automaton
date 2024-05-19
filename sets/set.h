#ifndef NF_AUTOMATA_SET_H
#define NF_AUTOMATA_SET_H

#define MAX_SIZE 100

typedef struct
{
  int elements[MAX_SIZE];
  int size;
  int mark;
} Set;

Set *create_set();
void insert_set(Set *set, int element);
void remove_set(Set *set, int element);
int contains(Set *set, int element);
void print_set(Set *set);

#endif // NF_AUTOMATA_SET_H
