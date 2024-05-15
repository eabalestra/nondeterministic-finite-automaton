#include <stdio.h>
#include <stdlib.h>
#include "../nf-automata/nf-automata.h"
#include "../grid/grid.h"

// Implementación de las funciones de grid

Grid *create_grid() {
    Grid *new_grid = (Grid *)malloc(sizeof(Grid));
    if (new_grid == NULL) {
        printf("Error: No se pudo asignar memoria para la grilla.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < DET_MAX_STATES; i++) {
        for (int j = 0; j < DET_MAX_SYMBOLS; j++) {
            new_grid->cell[i][j] = 0;
        }
    }

    return new_grid;
}