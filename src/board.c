#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // Para 'bool'
#include "board.h"
#include "rnd.h" // Para usar rnd_int

// Assinatura corrigida para receber o tamanho
void init_board(Board *b, int size) {
    // Aloca a memoria para o grid (assumindo a estrutura Cell *cells)
    b->rows = size;
    b->cols = size;
    // *** Aviso: Se a sua estrutura Board usa char grid[SIZE][SIZE], esta parte deve ser ignorada ou corrigida. ***
    
    // Assumindo que Board usa alocação dinamica:
    b->cells = malloc(size * size * sizeof(Cell));
    if (b->cells == NULL) {
        fprintf(stderr, "Erro de alocacao de memoria para o tabuleiro.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size * size; i++) {
        b->cells[i].state = CELL_WATER;
        b->cells[i].ship_id = -1;
    }
}

void print_board(const Board *b, int size, bool show_ships) {
    printf("  ");
    for (int c = 0; c < size; c++) printf(" %c", 'A' + c);
    printf("\n");

    for (int r = 0; r < size; r++) {
        printf("%2d", r + 1);
        for (int c = 0; c < size; c++) {
            Cell cell = b->cells[r * size + c];
            char symbol = '~';

            switch (cell.state) {
                case CELL_HIT:
                    symbol = 'X';
                    break;
                case CELL_MISS:
                    symbol = '.';
                    break;
                case CELL_SHIP:
                    if (show_ships) {
                        symbol = 'S';
                    } else {
                        symbol = '~';
                    }
                    break;
                case CELL_WATER:
                default:
                    symbol = '~';
                    break;
            }
            printf(" %c", symbol);
        }
        printf("\n");
    }
}

Cell *get_cell(Board *b, int r, int c) {
    if (r < 0 || r >= b->rows || c < 0 || c >= b->cols) return NULL;
    // Retorna o ponteiro para a célula no índice 1D: r * cols + c
    return &(b->cells[r * b->cols + c]);
}


void free_board_memory(Board *b) {
    if (b->cells != NULL) {
        free(b->cells);
        b->cells = NULL;
    }
    b->rows = 0;
    b->cols = 0;
}

void place_random_ship(Board *b, int ship_size, int ship_id) {
    int r, c;
    int horizontal; 
    int valid;

    while (1) {
        r = rnd_int(0, b->rows - 1);
        c = rnd_int(0, b->cols - 1);
        horizontal = rnd_int(0, 1);

        valid = 1;

        if (horizontal) {
            // Validação Horizontal
            if (c + ship_size > b->cols) { valid = 0; continue; }
            for (int i = 0; i < ship_size; i++) {
                if (get_cell(b, r, c + i)->state != CELL_WATER) {
                    valid = 0;
                    break;
                }
            }
        } else {
            // Validação Vertical
            if (r + ship_size > b->rows) { valid = 0; continue; }
            for (int i = 0; i < ship_size; i++) {
                if (get_cell(b, r + i, c)->state != CELL_WATER) {
                    valid = 0;
                    break;
                }
            }
        }
        
        // Posicionamento Final
        if (valid) {
            if (horizontal) {
                for (int i = 0; i < ship_size; i++) {
                    get_cell(b, r, c + i)->state = CELL_SHIP;
                    get_cell(b, r, c + i)->ship_id = ship_id;
                }
            } else {
                for (int i = 0; i < ship_size; i++) {
                    get_cell(b, r + i, c)->state = CELL_SHIP;
                    get_cell(b, r + i, c)->ship_id = ship_id;
                }
            }
            break; // Sai do loop
        }
    }
}