#ifndef IO_H
#define IO_H

#include "board.h"
#include <stdbool.h>

void io_read_line(const char *msg, char *buffer, int max_len);
int io_read_int(const char *msg);

int io_read_coord(const char *msg, int *row, int *col, int max_size, int max_cols);
void io_print(int *cells, int rows, int cols);

void print_shots_map(const Board *b, int size);

#endif