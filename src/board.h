#ifndef BOARD_H
#define BOARD_H

#define SIZE 10


typedef struct {
    char grid[SIZE][SIZE];
} Board;


void init_board(Board *b);
void print_board(Board *b);
void place_random_ship(Board *b, int ship_size);

#endif