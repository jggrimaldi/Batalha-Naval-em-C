#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "io.h"
#include "board.h" // Necessário para CellState

void io_read_line(const char *msg, char *buffer, int max_len) {
    printf("%s", msg);
    if (fgets(buffer, max_len, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

int io_read_int(const char *msg) {
    char buf[32];
    io_read_line(msg, buf, 32);
    return atoi(buf);
}

int io_read_coord(const char *msg, int *row, int *col, int max_size, int max_cols) {
    char buffer[10];
    io_read_line(msg, buffer, sizeof(buffer));

    if (strlen(buffer) < 2) {
        return 0;
    }

    char col_char = toupper(buffer[0]);
    *col = col_char - 'A';

    int line_num = atoi(&buffer[1]);
    *row = line_num - 1;

    if (*row < 0 || *row >= max_size || *col < 0 || *col >= max_cols) {
        printf("Coordenada invalida ou fora do tabuleiro (A1 - %c%d).\n", 'A' + max_cols - 1, max_size);
        return 0;
    }

    return 1;
}
void print_shots_map(const Board *b, int size) {
    printf("   "); 
    for (int i = 0; i < size; i++) {
        printf(" %c ", 'A' + i);
    }
    printf("\n");
    
    for (int r = 0; r < size; r++) {
        printf("%2d ", r + 1);
        for (int c = 0; c < size; c++) {
            Cell cell = b->cells[r * size + c];
            char symbol = '.'; // Padrao: Água (Desconhecido)

            switch (cell.state) {
                case CELL_SUNK: 
                    symbol = '#'; // Afundado (Visualização do Atacante)
                    break;
                case CELL_HIT:
                    symbol = 'X'; // Acerto
                    break;
                case CELL_MISS:
                    symbol = '~'; //Usa '~' para Tiro Errado/Miss
                    break;
                case CELL_WATER:
                case CELL_SHIP:
                default:
                    symbol = '.'; //Água não atirada (Desconhecido)
                    break;
            }
            printf(" %c ", symbol);
        }
        printf("\n");
    }
}

void io_print(int *cells, int rows, int cols) {
    printf(" ");
    for (int c = 0; c < cols; c++) printf("%c ", 'A' + c);
    printf("\n");


    for (int r = 0; r < rows; r++) {
        printf("%d ", r + 1);
        for (int c = 0; c < cols; c++) {
            printf("%d ", cells[r * cols + c]);
        }
        printf("\n");
    }
}