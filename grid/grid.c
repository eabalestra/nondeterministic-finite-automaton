#include "grid.h"
#include <stdio.h>
#include <stdlib.h>

Grid *create_grid()
{
  Grid *new_grid = (Grid *)malloc(sizeof(Grid));
  if (new_grid == NULL)
  {
    perror("Failed to allocate memory for Grid");
    exit(EXIT_FAILURE);
  }

  // Inicializar todos los elementos a un valor por defecto (por ejemplo, -1)
  for (int i = 0; i < DET_MAX_STATES; ++i)
  {
    for (int j = 0; j < DET_MAX_SYMBOLS; ++j)
    {
      new_grid->number[i][j] = -1;
    }
  }
  new_grid->no_end_cant_states = 0;
  new_grid->end_cant_states = 0;
  return new_grid;
}

void print_grid(Grid *grid, char alphabet[])
{
  if (grid == NULL)
  {
    printf("Grid is NULL\n");
    return;
  }
  printf("     ");
    for (int i = 0; alphabet[i] != -1 ; i++)
  {
    printf("%c   ", alphabet[i]);
  }
  printf("\n");

  int cant_states = grid->end_cant_states + grid->no_end_cant_states;
  // Imprimir la matriz principal
  for (int i = 0; i < cant_states; ++i)
  {
    printf("%3d", i); // Imprimir el número de la fila
    for (int j = 0; alphabet[j] != -1; ++j)
    {
      printf("%3d", grid->number[i][j]);
    }
    printf("\n");
  }
}

Grid *copy_grid(Grid *grid)
{
  if (grid == NULL)
  {
    printf("Grid is NULL\n");
    return NULL;
  }

  // Crear un nuevo Grid para almacenar la copia
  Grid *copy = create_grid();
  copy->end_cant_states = grid->end_cant_states;
  copy->no_end_cant_states = grid->no_end_cant_states;

  // Copiar los valores de cada elemento de grid a copy
  for (int i = 0; i < DET_MAX_STATES; ++i)
  {
    for (int j = 0; j < DET_MAX_SYMBOLS; ++j)
    {
      copy->number[i][j] = grid->number[i][j];
    }
  }

  return copy;
}

int grid_equals(Grid *grid1, Grid *grid2)
{
  if (grid1 == NULL || grid2 == NULL)
  {
    return 0;
  }

  if (grid1->no_end_cant_states != grid2->no_end_cant_states ||
      grid1->end_cant_states != grid2->end_cant_states)
  {
    return 0;
  }

  // Comprobar si los elementos de la matriz son iguales
  for (int i = 0; i < DET_MAX_STATES; ++i)
  {
    for (int j = 0; j < DET_MAX_SYMBOLS; ++j)
    {
      if (grid1->number[i][j] != grid2->number[i][j])
      {
        return 0;
      }
    }
  }

  return 1;
}

void calculate_grid(DFA *dfa, Grid *grid, Set *no_end_states[], Set *end_states[])
{
  for (int i = 0; i < dfa->states_cant; i++)
  {
    for (int j = 0; dfa->alphabet[j] != -1; j++)
    {
      // existe una transicion por i con el simbolo j?
      int elem = dfa->transitions[i][j];
      if (elem != -1)
      {
        int k = 0;
        // existe la transicion de i por el simbolo j, a que particion
        while (contains(no_end_states[k], elem) != 1 && contains(end_states[k], elem) != 1 && k < dfa->states_cant)
        {
          k++;
        }
        // ya se a que particion me lleva, pero es de la final o la noFinal, cambio en la grilla a cual dependiendo
        if (contains(no_end_states[k], elem) == 1)
        {
          grid->number[i][j] = no_end_states[k]->elements[0]; // Modifica la celda en la grilla
        }
        else
        {
          grid->number[i][j] = end_states[k]->elements[0];
        }
      }
    }
  }
}

void grid_clean(Grid *grid, Set *no_end_states[], Set *end_states[])
{
  for (int i = 0; i < grid->no_end_cant_states; i++)
  {
    Set *partition = no_end_states[i];
    if (partition->size > 1)
    {
      // elegir el representante de clase , aca elegimos siempre el mas chico de la clase
      int num_class = partition->elements[0];
      for (int j = 1; j < partition->size; j++)
      {
        int current_state = partition->elements[j];
        if (current_state < num_class)
        {
          num_class = current_state;
        }
      }
      // Todas las filas de la misma particion que no son del estado minimo se quedan llenas de -1
      for (int j = 0; j < partition->size; j++)
      {
        int current_state = partition->elements[j];
        if (current_state != num_class)
        {
          // Copiar la fila más pequeña a la fila actual
          for (int k = 0; k < DET_MAX_SYMBOLS; k++)
          {
            grid->number[current_state][k] = -1;
          }
        }
      }
    }
  }
  for (int i = 0; i < grid->end_cant_states; i++)
  {
    Set *partition = end_states[i];
    if (partition->size > 1)
    {
      // elegir el representante de clase , aca elegimos siempre el mas chico de la clase
      int num_class = partition->elements[0];
      for (int j = 1; j < partition->size; j++)
      {
        int current_state = partition->elements[j];
        if (current_state < num_class)
        {
          num_class = current_state;
        }
      }
      // Todas las filas de la misma particion que no son del estado minimo se quedan llenas de -1
      for (int j = 0; j < partition->size; j++)
      {
        int current_state = partition->elements[j];
        if (current_state != num_class)
        {
          // Copiar la fila más pequeña a la fila actual
          for (int k = 0; k < DET_MAX_SYMBOLS; k++)
          {
            grid->number[current_state][k] = -1;
          }
        }
      }
    }
  }
}

void init_parts(DFA *dfa, Set *no_end_states[], Set *end_states[])
{
  // Inicializa cada elemento del arreglo
  for (int i = 0; i < dfa->states_cant -1; i++)
  {
    no_end_states[i] = create_set();
    end_states[i] = create_set();
  }
  // División entre estados finales y no finales
  for (int states = 0; states < dfa->states_cant; states++)
  {
    if (!(dfa->states[states]->is_accepting == 1))
    {
      insert_set(no_end_states[0], states);
    }
    else
    {
      insert_set(end_states[0], states);
    }
  }
}

void calculate_parts(DFA *dfa, Set *no_end_states[], Set *end_states[], Grid *grid)
{
  // calculo de la parte no final
  int part = 0;
  int index = 0;
  int elim_no_end[dfa->states_cant];
  while (part < grid->no_end_cant_states)
  {
      for (int i = 0; i < dfa->states_cant; i++)
      {
        elim_no_end[i] = -1;
      }
    // fijo el primero y comparo sus fila contra todos los de la misma particion
    int primer = no_end_states[part]->elements[0];
    for (int i = 1; i < no_end_states[part]->size; i++)
    {
      // caso en que la particion tiene 1 elemento o cero, por lo que no la puedo dividir mas
      if (no_end_states[part]->size == 1 || no_end_states[part]->size == 0)
      {
        break;
      }
      int segundo = no_end_states[part]->elements[i];
      // letras o alfabeto
      for (int j = 0; dfa->alphabet[j] != -1; j++)
      {
        // elem1 devuelve transicionar desde numero "primer" por letra "j" y lo comparo contra lo mismo pero del segundo
        int elem1 = grid->number[primer][j];
        int elem2 = grid->number[segundo][j];
        // si son diferentes, entonces pertenecen a distintas particiones, por lo que saco el segundo
        // y lo pongo en siguiente particion
        if (elem1 != elem2)
        {
          elim_no_end[index] = segundo;
          index ++;
          insert_set(no_end_states[part + 1], segundo);
        }
      }
    }
    for (int i = 0; elim_no_end[i] != -1; i++)
    {
      remove_set(no_end_states[part], elim_no_end[i]);    
    }
    part++;
  }
  // calculo de la parte final
  int elim_end[dfa->states_cant];
  part = 0;
  index = 0;
  // esta parte es analoga pero con la otra parte, los estados finales
  while (part < grid->end_cant_states)
  {
    for (int i = 0; i < dfa->states_cant; i++)
    {
      elim_end[i] = -1;
    }
    int primer = end_states[part]->elements[0];
    for (int i = 1; i < end_states[part]->size; i++)
    {
      if (end_states[part]->size == 1 || end_states[part]->size == 0)
      {
        break;
      }
      int segundo = end_states[part]->elements[i];
      for (int j = 0; dfa->alphabet[j] != -1; j++)
      {
        int elem1 = grid->number[primer][j];
        int elem2 = grid->number[segundo][j];
        if (elem1 != elem2)
        {
          elim_end[index] = segundo;
          index ++;
          insert_set(end_states[part + 1], segundo);
        }
      }
    }
    for (int i = 0; elim_end[i] != -1; i++)
    {
      remove_set(end_states[part], elim_end[i]);    
    }
    part++;
  }
  // calculo la grid con la nueva particiones que se formaron
  //print_parts(no_end_states, end_states);

  calculate_grid(dfa, grid, no_end_states, end_states);
  print_grid(grid, dfa->alphabet);
}

void print_parts(Set *no_end_states[], Set *end_states[])
{
  printf("No End States:\n");
  for (int i = 0; i < 100; ++i)
  {
    if (no_end_states[i]->size == 0)
    {
      break;
    }
    printf("Partition %d: \n", i);
    print_set(no_end_states[i]);
  }
  printf("-------------------------------------------------------------------------\n");
  printf("End States:\n");
  for (int i = 0; i < 100; ++i)
  {
    if (end_states[i]->size == 0)
    {
      break;
    }
    printf("Partition %d: \n", i);
    print_set(end_states[i]);
  }
}

int sum_cant_parts(Grid *grid, Set *no_end_state[], Set *end_state[])
{
  int result = 0;
  for (int i = 0; i < grid->end_cant_states; i++)
  {
    if (end_state[i]->size != 0)
    {
      result++;
    }
  }
  for (int i = 0; i < grid->no_end_cant_states; i++)
  {
    if (no_end_state[i]->size != 0)
    {
      result++;
    }
  }
  return result;
}