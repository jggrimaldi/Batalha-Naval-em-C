#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    
    // Calcula a precisão do vencedor
    float precision = (winner->total_shots > 0) 
                      ? ((float)winner->successful_shots / winner->total_shots) * 100.0f 
                      : 0.0f;
                      
    printf("Stats de %s:\n", winner->nickname);
    printf("  Tiros: %d | Acertos: %d | Precisão: %.1f%%\n", 
           winner->total_shots, winner->successful_shots, precision);

    printf("\n--- Estado final dos tabuleiros ---\n");
    
    // Exibindo o tabuleiro do vencedor (seus navios)
    printf("\nTabuleiro de %s (seus navios):\n", winner->nickname);
    print_board(&(winner->board), g->board_size, true);

    // Exibindo o mapa de tiros do vencedor no perdedor
    printf("\nMapa de tiros de %s em %s:\n", winner->nickname, loser->nickname);
    print_shots_map(&(winner->shots), g->board_size);
}

void free_game_memory(Game *g) {
    // Libera memória de P1
    free_board_memory(&(g->p1.board));
    free_board_memory(&(g->p1.shots));
    free_fleet_memory(&(g->p1.fleet));
    
    // Libera memória de P2
    free_board_memory(&(g->p2.board));
    free_board_memory(&(g->p2.shots));
    free_fleet_memory(&(g->p2.fleet));

    printf("Memória do jogo liberada com sucesso.\n");
}


// --- Funções de Inicialização e Configuração ---

void config_game(Game *g) {
    printf("\n=== Configuracoes ===\n");
    
    // Tamanho do Tabuleiro
    g->board_size = io_read_int("Tamanho do tabuleiro (6-26): ");
    while (g->board_size < 6 || g->board_size > 26) {
        printf("Tamanho invalido. Escolha entre 6 e 26.\n");
        g->board_size = io_read_int("Tamanho do tabuleiro (6-26): ");
    }

    // Modo de Posicionamento
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

    // Leitura dos nicknames
    io_read_line("Informe apelido do Jogador 1: ", g->p1.nickname, 32);
    io_read_line("Informe apelido do Jogador 2: ", g->p2.nickname, 32);

    // Inicialização dos jogadores
    g->p1.total_shots = 0;
    g->p1.successful_shots = 0;
    g->p2.total_shots = 0;
    g->p2.successful_shots = 0;
    
    // Inicializa Tabuleiros (Board e Shots)
    init_board(&(g->p1.board), g->board_size);
    init_board(&(g->p1.shots), g->board_size);
    init_board(&(g->p2.board), g->board_size);
    init_board(&(g->p2.shots), g->board_size);

    // Inicializa e Carrega Frotas
    init_fleet(&(g->p1.fleet));
    init_fleet(&(g->p2.fleet));

    // Carrega navios para o Jogador 1
    add_ship(&(g->p1.fleet), "Porta-avioes", 5);
    add_ship(&(g->p1.fleet), "Encouracado", 4);
    add_ship(&(g->p1.fleet), "Cruzador_1", 3);
    add_ship(&(g->p1.fleet), "Cruzador_2", 3);
    add_ship(&(g->p1.fleet), "Destroyer_1", 2);
    add_ship(&(g->p1.fleet), "Destroyer_2", 2);

    // Carrega navios para o Jogador 2
    add_ship(&(g->p2.fleet), "Porta-avioes", 5);
    add_ship(&(g->p2.fleet), "Encouracado", 4);
    add_ship(&(g->p2.fleet), "Cruzador_1", 3);
    add_ship(&(g->p2.fleet), "Cruzador_2", 3);
    add_ship(&(g->p2.fleet), "Destroyer_1", 2);
    add_ship(&(g->p2.fleet), "Destroyer_2", 2);

    // Posicionamento
    if (g->placement_mode == 'A') {
        printf("\n[Frota de %s e %s posicionadas automaticamente]\n", g->p1.nickname, g->p2.nickname);
        place_fleet_random(&(g->p1.fleet), &(g->p1.board), g->board_size);
        place_fleet_random(&(g->p2.fleet), &(g->p2.board), g->board_size);
    } else { // 'M' - Manual
        printf("\nPosicionamento manual nao implementado. Usando automatico por enquanto.\n");
        place_fleet_random(&(g->p1.fleet), &(g->p1.board), g->board_size);
        place_fleet_random(&(g->p2.fleet), &(g->p2.board), g->board_size);
    }
}


// --- Funções do Fluxo Principal ---

void process_turn(Game *g, Player *shooter, Player *target) {
    int row, col;
    
    printf("\n-- Turno de %s --\n", shooter->nickname);

    // Exibir mapa de tiros
    printf("Mapa de tiros em %s:\n", target->nickname);
    print_shots_map(&(shooter->shots), g->board_size);

    // Coletar coordenada
    io_read_coord("Digite coordenada do tiro (ex.: E5): ", &row, &col, g->board_size, g->board_size);

    shooter->total_shots++;

    // Processar o tiro
    ShotResult result = fire_shot(&(shooter->shots), &(target->board), &(target->fleet), row, col);

    printf("Resultado: ");
    switch (result.type) {
        case RESULT_HIT:
            printf("ACERTOU no %s!\n", result.ship_name);
            shooter->successful_shots++;
            
            // Verifica se o navio afundou
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

    // Verificar condição de vitória
    if (check_win(&(target->fleet))) {
        g->game_over = true;
    }
}

void run_game_loop(Game *g) {
    Player *p_current, *p_opponent;

    while (!g->game_over) {
        // Define o jogador atual e o oponente
        if (g->current_player == 1) {
            p_current = &(g->p1);
            p_opponent = &(g->p2);
        } else {
            p_current = &(g->p2);
            p_opponent = &(g->p1);
        }

        // Processa o turno
        process_turn(g, p_current, p_opponent);

        // Se o jogo acabou
        if (g->game_over) {
            display_stats(g, p_current);
        } else {
            // Alterna para o próximo jogador
            toggle_player(g);
        }
    }
}

int main(void) {
    // Inicializa o rand()
    rnd_seed(); 
    
    Game game;
    int choice;
    
    // Define tamanho padrão
    game.board_size = 10; 
    game.placement_mode = 'A'; // Assume o modo automático como padrão

    do {
        printf("\n=== BATALHA NAVAL ===\n");
        printf("1) Novo jogo\n");
        printf("2) Configuracoes\n");
        printf("3) Sair\n");
        choice = io_read_int(">");

        switch (choice) {
            case 1:
                // Inicializa e roda o loop do jogo
                init_game(&game);
                run_game_loop(&game);
                free_game_memory(&game); 
                break;
            case 2:
                // Configura tamanho e modo de posicionamento
                config_game(&game);
                break;
            case 3:
                printf("Saindo do jogo. Ate logo!\n");
                break;
            default:
                printf("Opcao invalida. Escolha 1, 2 ou 3.\n");
        }
    } while (choice != 3);

    return 0;
}