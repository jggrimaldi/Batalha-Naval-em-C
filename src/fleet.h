#ifndef FLEET_H
#define FLEET_H

#include "board.h"

typedef struct {
    char name[20];
    int length;
    int hits;
    int placed;   // 0 = ainda não colocado no tabuleiro
} Ship;

typedef struct {
    Ship *ships;
    int count;     // quantidade atual de navios armazenados
} Fleet;

void init_fleet(Fleet *f);
void add_ship(Fleet *f, char *name, int length);
// Assinatura Corrigida: Adicionado 'size'
void place_fleet_random(Fleet *f, Board *b, int size);
void free_fleet_memory(Fleet *f);

#endif