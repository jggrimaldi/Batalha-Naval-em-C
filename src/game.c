#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "game.h"
#include "board.h"
#include "fleet.h"
#include "io.h"
#include "rnd.h"

// --- Funções Auxiliares ---

void toggle_player(Game *g) {
    g->current_player = (g->current_player == 1) ? 2 : 1;
}

bool check_win(const Fleet *fleet) {
    for (int i = 0; i < fleet->count; i++) {
        if (fleet->ships[i].hits < fleet->ships[i].length) {
            return false;
        }
    }
    return true;
}

void display_stats(const Game *g, const Player *winner) {
    const Player *loser = (winner == &(g->p1)) ? &(g->p2) : &(g->p1);

    printf("\n*** FIM DE JOGO ***\n");
    printf("🏆 Vencedor: %s\n", winner->nickname);
    
    float precision = (winner->total_shots > 0) 
                      ? ((float)winner->successful_shots / winner->total_shots) * 100.0f 
                      : 0.0f;
                      
    printf("Stats de %s:\n", winner->nickname);
    printf("  Tiros: %d | Acertos: %d | Precisão: %.1f%%\n", 
           winner->total_shots, winner->successful_shots, precision);

    printf("\n--- Estado final dos tabuleiros ---\n");
    
    printf("\nTabuleiro de %s (seus navios):\n", winner->nickname);
    print_board(&(winner->board), g->board_size, true);

    printf("\nMapa de tiros de %s em %s:\n", winner->nickname, loser->nickname);
    print_shots_map(&(winner->shots), g->board_size);
}

void free_game_memory(Game *g) {
    free_board_memory(&(g->p1.board));
    free_board_memory(&(g->p1.shots));
    free_fleet_memory(&(g->p1.fleet));
    
    free_board_memory(&(g->p2.board));
    free_board_memory(&(g->p2.shots));
    free_fleet_memory(&(g->p2.fleet));

    printf("Memória do jogo liberada com sucesso.\n");
}

// --- Funções de Inicialização e Configuração ---

void config_game(Game *g) {
    printf("\n=== Configuracoes ===\n");
    
    g->board_size = io_read_int("Tamanho do tabuleiro (6-26): ");
    while (g->board_size < 6 || g->board_size > 26) {
        printf("Tamanho invalido. Escolha entre 6 e 26.\n");
        g->board_size = io_read_int("Tamanho do tabuleiro (6-26): ");
    }

    char mode_input[10];
    io_read_line("Posicionamento (M)anual ou (A)utomatico? ", mode_input, 10);
    g->placement_mode = toupper(mode_input[0]);
    while (g->placement_mode != 'M' && g->placement_mode != 'A') {
        printf("Modo invalido. Use 'M' para Manual ou 'A' para Automatico.\n");
        io_read_line("Posicionamento (M)anual ou (A)utomatico? ", mode_input, 10);
        g->placement_mode = toupper(mode_input[0]);
    }

    printf("Configuracoes salvas.\n");
}

void init_game(Game *g) {
    g->game_over = false;
    g->current_player = 1;

    io_read_line("Informe apelido do Jogador 1: ", g->p1.nickname, 32);
    io_read_line("Informe apelido do Jogador 2: ", g->p2.nickname, 32);

    g->p1.total_shots = 0;
    g->p1.successful_shots = 0;
    g->p2.total_shots = 0;
    g->p2.successful_shots = 0;
    
    init_board(&(g->p1.board), g->board_size);
    init_board(&(g->p1.shots), g->board_size);
    init_board(&(g->p2.board), g->board_size);
    init_board(&(g->p2.shots), g->board_size);

    init_fleet(&(g->p1.fleet));
    init_fleet(&(g->p2.fleet));

    add_ship(&(g->p1.fleet), "Porta-avioes", 5);
    add_ship(&(g->p1.fleet), "Encouracado", 4);
    add_ship(&(g->p1.fleet), "Cruzador_1", 3);
    add_ship(&(g->p1.fleet), "Cruzador_2", 3);
    add_ship(&(g->p1.fleet), "Destroyer_1", 2);
    add_ship(&(g->p1.fleet), "Destroyer_2", 2);

    add_ship(&(g->p2.fleet), "Porta-avioes", 5);
    add_ship(&(g->p2.fleet), "Encouracado", 4);
    add_ship(&(g->p2.fleet), "Cruzador_1", 3);
    add_ship(&(g->p2.fleet), "Cruzador_2", 3);
    add_ship(&(g->p2.fleet), "Destroyer_1", 2);
    add_ship(&(g->p2.fleet), "Destroyer_2", 2);

    if (g->placement_mode == 'A') {
        printf("\n[Frota de %s e %s posicionadas automaticamente]\n", g->p1.nickname, g->p2.nickname);
        place_fleet_random(&(g->p1.fleet), &(g->p1.board), g->board_size);
        place_fleet_random(&(g->p2.fleet), &(g->p2.board), g->board_size);
    } else {
        printf("\nPosicionamento manual nao implementado. Usando automatico por enquanto.\n");
        place_fleet_random(&(g->p1.fleet), &(g->p1.board), g->board_size);
        place_fleet_random(&(g->p2.fleet), &(g->p2.board), g->board_size);
    }
}

void process_turn(Game *g, Player *shooter, Player *target) {
    int row, col;
    
    printf("\n-- Turno de %s --\n", shooter->nickname);

    printf("Mapa de tiros em %s:\n", target->nickname);
    print_shots_map(&(shooter->shots), g->board_size);

    io_read_coord("Digite coordenada do tiro (ex.: E5): ", &row, &col, g->board_size, g->board_size);

    shooter->total_shots++;

    ShotResult result = fire_shot(&(shooter->shots), &(target->board), &(target->fleet), row, col);

    printf("Resultado: ");
    switch (result.type) {
        case RESULT_HIT:
            printf("ACERTOU no %s!\n", result.ship_name);
            shooter->successful_shots++;
            
            if (result.is_sunk) {
                printf("AFUNDOU %s!\n", result.ship_name);
            }
            break;
        case RESULT_MISS:
            printf("AGUA.\n");
            break;
        case RESULT_INVALID:
        case RESULT_REPEATED:
            printf("TIRO REPETIDO ou INVALDO. Perdeu a vez.\n"); 
            break;
    }

    if (check_win(&(target->fleet))) {
        g->game_over = true;
    }
}

void run_game_loop(Game *g) {
    Player *p_current, *p_opponent;

    while (!g->game_over) {
        if (g->current_player == 1) {
            p_current = &(g->p1);
            p_opponent = &(g->p2);
        } else {
            p_current = &(g->p2);
            p_opponent = &(g->p1);
        }

        process_turn(g, p_current, p_opponent);

        if (g->game_over) {
            display_stats(g, p_current);
        } else {
            toggle_player(g);
        }
    }
}

ShotResult fire_shot(Board *shots, Board *target_board, Fleet *target_fleet, int row, int col) {
    ShotResult result;
    result.is_sunk = false;
    strcpy(result.ship_name, "Nenhum");

    // 1. Validar Limites (Se não for validado em io.c)
    if (row >= target_board->rows || col >= target_board->cols || row < 0 || col < 0) {
        result.type = RESULT_INVALID;
        return result;
    }

    int index = row * target_board->cols + col;
    Cell *target_cell = &(target_board->cells[index]);
    Cell *shot_cell = &(shots->cells[index]);

    // 2. Verificar Tiro Repetido
    if (shot_cell->state != CELL_WATER) {
        result.type = RESULT_REPEATED;
        return result;
    }

    // --- 3. Processar o Tiro ---

    if (target_cell->state == CELL_SHIP) {
        // ACERTOU
        result.type = RESULT_HIT;
        
        // Registrar o tiro
        target_cell->state = CELL_HIT;
        shot_cell->state = CELL_HIT;

        // 4. Lógica de Frota: Encontrar o Navio, Registrar Hit e Verificar Afundamento
        int ship_id = target_cell->ship_id;
        
        // Verificação de segurança, garantindo que o ID é válido (0 a count-1)
        if (ship_id >= 0 && ship_id < target_fleet->count) {
            Ship *hit_ship = &(target_fleet->ships[ship_id]);
            
            // Incrementa o contador de acertos
            hit_ship->hits++;
            
            // Copia o nome do navio
            strncpy(result.ship_name, hit_ship->name, 20);
            result.ship_name[19] = '\0';
            
            // Verifica Afundamento
            if (hit_ship->hits >= hit_ship->length) {
                result.is_sunk = true;
            }
        }
        
    } else {
        // ERROU (ÁGUA)
        result.type = RESULT_MISS;

        // Registrar o tiro
        target_cell->state = CELL_MISS;
        shot_cell->state = CELL_MISS;
    }

    return result;
}