#define MAX_SIZE 100
#define DET_MAX_STATES 100
#define DET_MAX_SYMBOLS 27

typedef struct {
    int cell[DET_MAX_STATES][DET_MAX_SYMBOLS]; 
} Grid;

Grid *create_grid();

void printf_grid(Grid grid);