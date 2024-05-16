#include "df-automata.h"

#include <stdio.h>
#include <stdlib.h>
#include "../nf-automata/nf-automata.h"
#define NUM_SETS 10
#define true 1
#define false 0

DFA *create_dfa()
{
  DFA *dfa = malloc(sizeof(DFA));
  for (int from = 0; from < DET_MAX_STATES; from++)
  {
    for (int symbol = 0; symbol < DET_MAX_SYMBOLS; symbol++)
    {
      dfa->transitions[from][symbol] = -1;
    }
  }
  dfa->initial_state = 0;
  dfa->states_cant = 0;
  return dfa;
}

int det_add_state(DFA *dfa, State *state)
{
  int last_index = dfa->states_cant;

  if (state->enteros->size == 0)
  {
    return -1;
  }

  for (int i = 0; i < last_index; i++)
  {
    if (equals(dfa->states[i], state))
    { // dont add it if exists
      return i;
    }
  }

  dfa->states[last_index] = state;
  dfa->states_cant = last_index + 1;
  return last_index;
}

void det_add_transition(DFA *dfa, int from, int to, char symbol)
{
  dfa->transitions[from][symbol - 'a'] = to;
}

void det_set_accepting(DFA *dfa, int index)
{
  State *state = dfa->states[index];
  state->is_accepting = 1;
}

State *det_transition(DFA *dfa, int current_state, char symbol)
{
  int symbol_index = symbol - 'a'; // Convert the symbol to an index
  int next_state_index = dfa->transitions[current_state][symbol_index];
  if (next_state_index == -1)
  {
    return NULL;
  }

  return dfa->states[next_state_index];
}

void set_accepting(DFA *dfa, NFA *nfa)
{
  for (int i = 0; i < dfa->states_cant; i++)
  {
    for (int j = 0; j < DET_MAX_SYMBOLS; j++)
    {
      if (nfa->is_accepting[j] && contains(dfa->states[i]->enteros, j))
      {
        det_set_accepting(dfa, i);
      }
    }
  }
}

void print_dfa(DFA *dfa)
{

  for (int i = 0; i < dfa->states_cant; i++)
  {
    State *state = dfa->states[i];
    if (state->is_accepting)
    {
      print_state(state);
      printf("IS ACCEPTING\n");
    }

    for (int symbol = 0; symbol < DET_MAX_SYMBOLS; symbol++)
    {
      State *goal = det_transition(dfa, i, 'a' + symbol);
      if (goal != NULL)
      {
        printf("-----------------------------------\nTransition from ");
        print_state(state);
        printf("by '%c' to ", 'a' + symbol);
        print_state(goal);
      }
    }
  }
}

void dfa_to_dot(DFA *dfa, const char *filename)
{
  FILE *file = fopen(filename, "w");

  if (file == NULL)
  {
    printf("Error creating dot file: %s\n", filename);
    return;
  }

  fprintf(file, "digraph{\n");
  fprintf(file, "    rankdir=LR;\n");
  fprintf(file, "    inic[shape=point];\n");
  fprintf(file, "\n    inic->q%i;\n\n", dfa->initial_state);

  for (int from = 0; from < dfa->states_cant; from++)
  {
    for (int symbol = 0; symbol < DET_MAX_SYMBOLS; symbol++)
    {
      int data = dfa->transitions[from][symbol];
      if (data != -1)
      {
        fprintf(file, "    q%d->q%d [label=\"%c\"];\n", from, data, 'a' + symbol);
      }
    }
    if (dfa->states[from]->is_accepting == 1)
    {
      fprintf(file, "\n    q%d[shape=doublecircle];\n", from);
    }
  }
  fprintf(file, "}");
  for (int state = 0; state < dfa->states_cant; state++)
  {
    fprintf(file, "\n//State q%i: {", state);
    for (int i = 0; i < dfa->states[state]->enteros->size; i++)
    {
      fprintf(file, "%d", dfa->states[state]->enteros->elements[i]);
      if (i < dfa->states[state]->enteros->size - 1)
      {
        fprintf(file, ", ");
      }
    }
    fprintf(file, "}\n");
  }

  fclose(file);
}

//Esto tiene que ir en grid.c pero por ahora uso todo aca para que ande sin los include
//************************************************************************************************************************
Grid *create_grid() {
    Grid *new_grid = (Grid *)malloc(sizeof(Grid));
    if (new_grid == NULL) {
        perror("Failed to allocate memory for Grid");
        exit(EXIT_FAILURE);
    }

    // Inicializar todos los elementos a un valor por defecto (por ejemplo, 0)
    for (int i = 0; i < DET_MAX_STATES; ++i) {
        for (int j = 0; j < DET_MAX_SYMBOLS; ++j) {
            new_grid->number[i][j] = 0;
        }
    }
    new_grid->no_end_cant_states = 0;
    new_grid->end_cant_states = 0;
    return new_grid;
}

void grid_print(Grid *grid) {
    if (grid == NULL) {
        printf("Grid is NULL\n");
        return;
    }
    printf("     ");
    for (int j = 'a'; j <= 'e'; ++j) {
        printf("%c  ", j);
    }
    printf("\n");

    // Imprimir la matriz principal
    for (int i = 0; i < 5; ++i) {
        printf("%3d", i); // Imprimir el número de la fila
        for (int j = 0; j < 5; ++j) {
            printf("%3d", grid->number[i][j]);
        }
        printf("\n");
    }
}

Grid *grid_copy(Grid *grid) {
    if (grid == NULL) {
        printf("Grid is NULL\n");
        return NULL;
    }

    // Crear un nuevo Grid para almacenar la copia
    Grid *copy = create_grid();
    copy->end_cant_states = grid->end_cant_states;
    copy->no_end_cant_states = grid->no_end_cant_states;

    // Copiar los valores de cada elemento de grid a copy
    for (int i = 0; i < DET_MAX_STATES; ++i) {
        for (int j = 0; j < DET_MAX_SYMBOLS; ++j) {
            copy->number[i][j] = grid->number[i][j];
        }
    }

    return copy;
}

int grid_equals(Grid *grid1, Grid *grid2) {
    if (grid1 == NULL || grid2 == NULL) {
        return 0;
    }

    if (grid1->no_end_cant_states != grid2->no_end_cant_states || 
        grid1->end_cant_states != grid2->end_cant_states) {
        return 0;
    }

    // Comprobar si los elementos de la matriz son iguales
    for (int i = 0; i < DET_MAX_STATES; ++i) {
        for (int j = 0; j < DET_MAX_SYMBOLS; ++j) {
            if (grid1->number[i][j] != grid2->number[i][j]) {
                return 0;
            }
        }
    }

    return 1;
}

void calculate_grid(DFA *dfa, Grid *grid, Set *no_end_states[],Set *end_states[]){
  for (int i = 0; i < DET_MAX_STATES; i++) {
    for (int j = 0; j < DET_MAX_SYMBOLS; j++) {
      //existe una transicion por i con el simbolo j?
      int elem = dfa->transitions[i][j];
      if (elem != -1) {
        int k = 0;
        //existe la transicion de i por el simbolo j, a que particion 
        while(contains(no_end_states[k],elem) != 1  && contains(end_states[k], elem) != 1 && k < dfa->states_cant){
          k++;
        }
        //ya se a que particion me lleva, pero es de la final o la noFinal, cambio en la grilla a cual dependiendo
        if(contains(no_end_states[k],elem) == 1){
          grid->number[i][j] = no_end_states[k]->elements[0]; // Modifica la celda en la grilla
        }else{
          grid->number[i][j] = end_states[k]->elements[0];
        }
      }
    }
  }
}

void grid_clean(Grid *grid, Set *no_end_states[], Set *end_states[]) {
  for (int i = 0; i < grid->no_end_cant_states; i++) {
    Set *partition = no_end_states[i];
    if (partition->size > 1) {
      // elegir el representante de clase , aca elegimos siempre el mas chico de la clase
      int num_class = partition->elements[0];
      for (int j = 1; j < partition->size; j++) {
        int current_state = partition->elements[j];
        if (current_state < num_class) {
          num_class = current_state;
        }
      }
      // Todas las filas de la misma particion que no son del estado minimo se quedan llenas de -1
      for (int j = 0; j < partition->size; j++) {
        int current_state = partition->elements[j];
        if (current_state != num_class) {
          // Copiar la fila más pequeña a la fila actual
          for (int k = 0; k < DET_MAX_SYMBOLS; k++) {
            grid->number[current_state][k] = -1;
          }
        }
      } 
    }
  }
  for (int i = 0; i < grid->end_cant_states; i++) {
    Set *partition = end_states[i];
    if (partition->size > 1) {
      // elegir el representante de clase , aca elegimos siempre el mas chico de la clase
      int num_class = partition->elements[0];
      for (int j = 1; j < partition->size; j++) {
        int current_state = partition->elements[j];
        if (current_state < num_class) {
          num_class = current_state;
        }
      }
      // Todas las filas de la misma particion que no son del estado minimo se quedan llenas de -1
      for (int j = 0; j < partition->size; j++) {
        int current_state = partition->elements[j];
        if (current_state != num_class) {
          // Copiar la fila más pequeña a la fila actual
          for (int k = 0; k < DET_MAX_SYMBOLS; k++) {
            grid->number[current_state][k] = -1;
          }
        }
      } 
    }
  }
}


//************************************************************************************************************************

//Implementacion de minimization, hay que factorizarlo y quitarle las cosas que estan de mas
//MUCHO COMENTARIO, SACARLOS ANTES DE ENTREGAR, PREGUNTENME SI NO ENTIENDEN ALGO:) ATTE:YO
DFA *minimization(DFA *dfa){
  Set *no_end_states[NUM_SETS]; // Arreglo de punteros a conjuntos
  Set *end_states[NUM_SETS];

  //aca hacemos la primera division entre Set de estados finales y los no finales
  init_parts(dfa, no_end_states, end_states);

  //creamos la tabla de transiciiones
  Grid *grid = create_grid();
  //necesito para los ciclos cuantos estados tiene el conjunto de finales y NO-finales, asiq los guarde en grid:)
  grid->no_end_cant_states = no_end_states[0]->size;
  grid->end_cant_states = end_states[0]->size;
  //Calcula a que clase de equivalencia va cada stado por cada letra
  calculate_grid(dfa,grid, no_end_states, end_states);
  //quitar esto antes de la entrega finar
  grid_print(grid);
  printf("-------------------------------------------------------------------------\n");
  // hago una copia de la grilla para poder comparar con la nueva que voy a generar y saber cuando para el ciclo y que
  // ya no puedo separar mas las equivalencias
  Grid *copy = create_grid();
  //condicion de finalizacion
  while(grid_equals(copy,grid) != 1){
    //actualizo la copia
    copy = grid_copy(grid);
    //aca dentro del calculate, a partir de grid, separo las clases de equivalencia y vuelvo a calcular grid
    calculate_parts(dfa, no_end_states, end_states, grid);
  }
  //sacar esto pero es como quedan las particiones al finalizar el ciclo
  print_parts(no_end_states, end_states); 

  //construir el automata en base a grid
  grid_clean(grid, no_end_states, end_states);
  grid_print(grid);
  int cant_parts = sum_cant_parts(grid, no_end_states,end_states);
  printf("Cantidad total de particiones: %d \n", cant_parts);
  DFA *result = create_dfa();
  printf("q%d->q%d [label=%c];\n",0,grid->number[0][0], 'a');
  /**
  for (int i = 0; i < cant_parts; i++)
  {
    if (grid->number[i][0] != -1)
    {     
      for (int j = 'a'; j <= 'z'; j++)
      {
        //esto no anda, hay que arreglar lo del malloc
        det_add_transition(result, i, grid->number[i][j-'a'], j);
      }
    }
  }
  print_dfa(result);*/
  print_parts(no_end_states,end_states);
  return result;
}
int count_parts(Set *no_end_states[], Set *end_states[]);

void init_parts(DFA *dfa, Set *no_end_states[], Set *end_states[]){
  // Inicializa cada elemento del arreglo
  for (int i = 0; i < NUM_SETS; i++) {
      no_end_states[i] = create_set();
      end_states[i] = create_set();
  }
  // División entre estados finales y no finales
  for (int  states = 0; states < dfa->states_cant ; states++) {
    if (!(dfa->states[states]->is_accepting == 1)) {
      insert_set(no_end_states[0], states);
    } else {
      insert_set(end_states[0], states);
    }
  }
}


void calculate_parts(DFA *dfa, Set *no_end_states[], Set *end_states[], Grid *grid){
  //calculo de la parte no final
  int part = 0;
  while (part <= grid->no_end_cant_states)
  {
    //fijo el primero y comparo sus fila contra todos los de la misma particion
    int primer = no_end_states[part]->elements[0];
    for (size_t i = 1; i < no_end_states[part]->size ; i++)
    {
      //caso en que la particion tiene 1 elemento o cero, por lo que no la puedo dividir mas
      if(no_end_states[part]->size == 1  || no_end_states[part]->size == 0){
        break;
      }
      int segundo = no_end_states[part]->elements[i];
      //letras o alfabeto
      for (size_t j = 1; j <= 4 ; j++)
      {
        //elem1 devuelve transicionar desde numero "primer" por letra "j" y lo comparo contra lo mismo pero del segundo
        int elem1 = grid->number[primer][j-1];
        int elem2 = grid->number[segundo][j-1];
        //si son diferentes, entonces pertenecen a distintas particiones, por lo que saco el segundo 
        //y lo pongo en siguiente particion
        if(elem1 != elem2){
          remove_set(no_end_states[part], segundo);
          insert_set(no_end_states[part+1], segundo);
        }
      }
    }
    part++;
  }
  //calculo de la parte final
  part = 0;
  //esta parte es analoga pero con la otra parte, los estados finales
  while (part <= grid->end_cant_states)
  {
    int primer = end_states[part]->elements[0];
    for (size_t i = 1; i < end_states[part]->size; i++)
    {
      if(end_states[part]->size == 1  || end_states[part]->size == 0){
        break;
      }
      int segundo = end_states[part]->elements[i];
      for (size_t j = 1; j <= 4 ; j++)
      {
        int elem1 = grid->number[primer][j-1];
        int elem2 = grid->number[segundo][j-1];
        if(elem1 != elem2){
          printf("%d", elem2);
          remove_set(end_states[part], segundo);
          insert_set(end_states[part+1], segundo);
        }
      }
    }
    part++;
  }
  //calculo la grid con la nueva particiones que se formaron
  calculate_grid(dfa, grid, no_end_states, end_states);
  //quitar esto
  print_parts(no_end_states, end_states); 
  grid_print(grid);
    printf("-------------------------------------------------------------------------\n");

}

//printea las particiones EZ
void print_parts(Set *no_end_states[], Set *end_states[]) {
    printf("No End States:\n");
    for (int i = 0; i < 100; ++i) {
        if (no_end_states[i]->size == 0)
        {
          break;
        }
        printf("Partition %d: \n", i);
        print_set(no_end_states[i]);

    }
    printf("-------------------------------------------------------------------------\n");
    printf("End States:\n");
    for (int i = 0; i < 100; ++i) {
        if (end_states[i]->size == 0)
        {
          break;
        }
        printf("Partition %d: \n", i);
        print_set(end_states[i]);
    }
}


int sum_cant_parts(Grid *grid, Set *no_end_state[], Set *end_state[]){
  int result = 0;
  for (int i = 0; i < grid->end_cant_states; i++)
  {
    if (end_state[i]->size != 0)
    {
      result ++;
    }
    }
  for (int i = 0; i < grid->no_end_cant_states; i++)
  {
    if (no_end_state[i]->size != 0)
    {
      result ++;
    }
  }
  return result;
}