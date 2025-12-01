#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "io.h"


void io_read_line(const char *msg, char *buffer, int max_len) {
printf("%s", msg);
fgets(buffer, max_len, stdin);
buffer[strcspn(buffer, "\n")] = '\0';
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