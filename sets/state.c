#include "state.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

State *create_state()
{
  State *new_state = malloc(sizeof(State));
  new_state->enteros = create_set();

  return new_state;
}

void add_to_state(State *state, int elem)
{
  insert_set(state->enteros, elem);
}

int equals(State *state1, State *state2)
{

  if (state1 == NULL || state2 == NULL)
  {
    return 0;
  }

  if (state1->enteros->size != state2->enteros->size)
  {
    return 0;
  }

  if (state1->enteros == state2->enteros)
  {
    return 1; // Los conjuntos son iguales
  }

  for (int i = 0; i < state1->enteros->size; i++)
  {
    int found = 0;
    for (int j = 0; j < state2->enteros->size; j++)
    {
      if (state1->enteros->elements[i] == state2->enteros->elements[j])
      {
        found = 1;
        break;
      }
    }
    if (!found)
    {
      return 0;
    }
  }

  return 1;
}

void print_state(State *state)
{
  if (state == NULL)
  {
    printf("Estado no válido\n");
    return;
  }

  printf("State: {");

  for (int i = 0; i < state->enteros->size; i++)
  {
    printf("%d", state->enteros->elements[i]);
    if (i < state->enteros->size - 1)
    {
      printf(", ");
    }
  }
  printf("}\n");

  if (state->is_accepting)
  {
    printf("ACCEPTING STATE");
  }
}
