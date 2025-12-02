#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fleet.h"
#include "rnd.h"

void init_fleet(Fleet *f) {
    f->ships = NULL;
    f->count = 0;
}

// Implementação usando realloc
void add_ship(Fleet *f, char *name, int length) {
    f->count++;
    f->ships = (Ship *)realloc(f->ships, f->count * sizeof(Ship));

    if (f->ships == NULL) {
        fprintf(stderr, "Erro ao realocar memoria para a frota.\n");
        exit(EXIT_FAILURE);
    }

    Ship *new_ship = &(f->ships[f->count - 1]);
    strncpy(new_ship->name, name, 20);
    new_ship->name[19] = '\0';
    new_ship->length = length;
    new_ship->hits = 0;
    new_ship->placed = 0;
}

void free_fleet_memory(Fleet *f) {
    if (f->ships != NULL) {
        free(f->ships);
        f->ships = NULL;
    }
    f->count = 0;
}

// --- Lógica de Posicionamento ---

void place_fleet_random(Fleet *f, Board *b, int size) {
    // size não é usado diretamente aqui, mas é mantido pela assinatura.
    (void)size; 
    
    // Itera sobre cada navio na frota
    for (int i = 0; i < f->count; i++) {
        // Delega o posicionamento ao board.c, passando o ship_id (índice 'i')
        place_random_ship(b, f->ships[i].length, i);
        
        // Marca o navio como colocado na frota
        f->ships[i].placed = 1;
    }
}