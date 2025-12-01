#include <stdio.h>
#include <stdlib.h>
#include "board.h"

void init_board(Board *b) {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            b->grid[i][j] = '~';  
        }
    }
}

void print_board(Board *b) {
    int i, j;

    printf("  ");
    for (i = 0; i < SIZE; i++)
        printf("%d ", i);
    printf("\n");

    for (i = 0; i < SIZE; i++) {
        printf("%d ", i);
        for (j = 0; j < SIZE; j++) {
            printf("%c ", b->grid[i][j]);
        }
        printf("\n");
    }
}

void place_random_ship(Board *b, int ship_size) {
    int row, col;
    int i;
    int horizontal;

    while (1) {
        row = rand() % SIZE;
        col = rand() % SIZE;
        horizontal = rand() % 2;  // 0 vertical, 

        if (horizontal) {
            if (col + ship_size < SIZE) {
                for (i = 0; i < ship_size; i++)
                    if (b->grid[row][col + i] != '~')
                        break;

                if (i == ship_size) {
                    for (i = 0; i < ship_size; i++)
                        b->grid[row][col + i] = '#';
                    break;
                }
            }
        } else {
            if (row + ship_size < SIZE) {
                for (i = 0; i < ship_size; i++)
                    if (b->grid[row + i][col] != '~')
                        break;

                if (i == ship_size) {
                    for (i = 0; i < ship_size; i++)
                        b->grid[row + i][col] = '#';
                    break;
                }
            }
        }
    }
}