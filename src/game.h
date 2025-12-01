#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "fleet.h"
#include "io.h"
#include "rnd.h"
#include <stdbool.h>

//ENUMS e STRUCTS para a Lógica de Tiro

typedef enum {
    RESULT_HIT,
    RESULT_MISS,
    RESULT_REPEATED,
    RESULT_INVALID
} ShotType;

typedef struct {
    ShotType type;
    char ship_name[20];
    bool is_sunk;
} ShotResult;

//Estruturas Principais

typedef struct {
    Board board;
    Board shots;
    Fleet fleet;
    char nickname[32];
    int total_shots;
    int successful_shots;
} Player;

typedef struct {
    Player p1, p2;
    int current_player;
    bool game_over;
    int board_size;
    char placement_mode;
} Game;


// Inicialização e Configuração
void init_game(Game *g);
void config_game(Game *g);

// Fluxo Principal
void run_game_loop(Game *g);
void process_turn(Game *g, Player *shooter, Player *target);

// Funções Auxiliares
void toggle_player(Game *g);
bool check_win(const Fleet *fleet);
void display_stats(const Game *g, const Player *winner);
void free_game_memory(Game *g);

// Assinatura da função de tiro (implementação em outro módulo)
ShotResult fire_shot(Board *shots, Board *target_board, Fleet *target_fleet, int row, int col);

#endif 