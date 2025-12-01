#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "io.h"
#include "rnd.h"

int main(void) {
    // Inicializa a semente do gerador de números aleatórios
    rnd_seed(); 
    
    Game game;
    int choice;
    
    // Define um tamanho padrão e modo de posicionamento iniciais (podem ser alterados em Configurações)
    game.board_size = 10; 
    game.placement_mode = 'A'; // A = Automatico
    
    do {
        printf("\n=== BATALHA NAVAL ===\n");
        printf("1) Novo jogo\n");
        printf("2) Configuracoes\n");
        printf("3) Sair\n");
        
        choice = io_read_int(">");

        switch (choice) {
            case 1:
                // 1. Inicializa o jogo com as configuracoes atuais
                init_game(&game);
                
                // 2. Roda o loop principal do jogo (turnos)
                run_game_loop(&game);
                
                // 3. Libera a memoria alocada para os tabuleiros e frotas
                free_game_memory(&game); 
                break;
            case 2:
                // Permite ao usuario alterar o tamanho do tabuleiro e o modo de posicionamento
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