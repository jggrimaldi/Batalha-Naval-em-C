#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "io.h"
#include "rnd.h"

int main(void) {
    // Inicializa o rand()
    rnd_seed(); 
    
    Game game;
    int choice;
    
    // Define tamanho padrão
    game.board_size = 10; 
    game.placement_mode = 'A';

    do {
        printf("\n=== BATALHA NAVAL ===\n");
        printf("1) Novo jogo\n");
        printf("2) Configuracoes\n");
        printf("3) Sair\n");
        
        choice = io_read_int(">");

        switch (choice) {
            case 1:
                init_game(&game);
                run_game_loop(&game);
                free_game_memory(&game); 
                break;
            case 2:
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