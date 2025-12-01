#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fleet.h"
#include "board.h"

// Inicializa a estrutura da frota
void init_fleet(Fleet *f) {
    f->ships = NULL;
    f->count = 0;
}

// Adiciona um navio na lista (usa realloc)
void add_ship(Fleet *f, char *name, int length) {

    // aumenta o vetor para comportar mais 1 navio
    f->ships = realloc(f->ships, (f->count + 1) * sizeof(Ship));

    strcpy(f->ships[f->count].name, name);
    f->ships[f->count].length = length;
    f->ships[f->count].hits = 0;
    f->ships[f->count].placed = 0;

    f->count++;   // agora temos +1 navio armazenado
}

// Posiciona todos os navios aleatoriamente usando a função do board
void place_fleet_random(Fleet *f, Board *b) {

    for (int i = 0; i < f->count; i++) {
        place_random_ship(b, f->ships[i].length);
        f->ships[i].placed = 1;
    }
}
