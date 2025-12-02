#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include <stdlib.h> 

// Estruturas fornecidas
typedef enum { CELL_WATER, CELL_SHIP, CELL_HIT, CELL_MISS } CellState;

typedef struct {
    CellState state;
    int ship_id; // -1 se não houver navio
} Cell;

typedef struct {
    int rows, cols;
    Cell *cells; // malloc(rows * cols)
} Board;

// Assinaturas Corrigidas: recebem 'size'. print_board recebe 'const'.
void init_board(Board *b, int size);
void print_board(const Board *b, int size, bool show_ships);
void free_board_memory(Board *b);
void place_random_ship(Board *b, int ship_size, int ship_id);

#endif