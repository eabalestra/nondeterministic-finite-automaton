#include "set.h"
#include <stdlib.h>
#include "stdio.h"

Set *create_set()
{
  Set *new_set = (Set *)malloc(sizeof(Set));
  new_set->size = 0;
  new_set->mark = 0;
  return new_set;
}

void insert_set(Set *set, int element)
{
  if (!contains(set, element))
  {
    set->elements[set->size] = element;
    set->size++;
  }
}

void mark(Set *set, int bool){
  set->mark = bool;
}

int contains(Set *set, int element)
{
  for (int i = 0; i < set->size; i++)
  {
    if (set->elements[i] == element)
    {
      return 1;
    }
  }
  return 0;
}

void print_set(Set *set)
{
  printf("{");
  for (int i = 0; i < set->size; i++)
  {
    printf("%d ", set->elements[i]);
  }
  printf("}\n");
}