🛳️ Batalha Naval em C

Projeto – Curso de Sistemas de Informação
CESAR School
Professor: João Victor Tinoco

📌 Visão Geral

Este projeto implementa o jogo Batalha Naval utilizando exclusivamente linguagem C e as bibliotecas padrão permitidas.
O objetivo é exercitar structs, ponteiros, alocação dinâmica (malloc/realloc), modularização e controle de fluxo.

O jogo funciona totalmente no terminal (CLI) e permite posicionamento automático ou manual dos navios, além da alternância de turnos entre dois jogadores.

📁 Estrutura do Projeto
/src
    board.h
    board.c
    fleet.h
    fleet.c
    rnd.h
    rnd.c
    io.h
    io.c
    game.h
    game.c
    main.c
Makefile
README.md

🔧 Módulos
Módulo	Responsabilidade
board.c / board.h	Gerencia o tabuleiro: inicialização, impressão e posicionamento aleatório.
fleet.c / fleet.h	Gerencia a frota: criação dos navios, armazenamento e posicionamento.
rnd.c / rnd.h	Geração de números aleatórios.
io.c / io.h	Rotinas de entrada e saída no terminal (ler coordenadas, inteiros, strings).
game.c / game.h	Loop principal do jogo, controle de turnos, regras e disparos.
main.c	Ponto de entrada: menu inicial, configurações e chamada do jogo.
🎮 Regras do Jogo

O tabuleiro padrão é 10×10, podendo ser configurado entre 6 e 26.

Frota mínima implementada:

1 Porta-aviões (5 células)

1 Encouraçado (4 células)

2 Cruzadores (3 células)

2 Destroyers (2 células)

Cada jogador:

Informa um apelido.

Recebe um tabuleiro.

Posiciona a frota manualmente ou automaticamente.

Atira informando coordenadas como B5.

Estados das células:

~ água

# navio

X acerto

. tiro errado

O jogo termina quando toda a frota de um jogador for destruída.

▶️ Como Compilar e Executar

Este projeto inclui um Makefile simplificado.

Para compilar:
make

Para executar:
./batalha

Para limpar arquivos .o e binário:
make clean

📦 Dependências

Apenas bibliotecas padrão permitidas:

<stdio.h>

<stdlib.h>

<string.h>

<ctype.h>

<stdbool.h>

<time.h>

Não há dependências externas.

🧠 Conceitos Aplicados

✔ Uso de structs para modelar tabuleiros, navios, frotas e jogadores
✔ Uso explícito de malloc, realloc e free
✔ Modularização e separação de responsabilidades
✔ Manipulação por ponteiros
✔ Controle de fluxo (loops, menus, validações)
✔ Leitura robusta de coordenadas e entradas do usuário

👥 Equipe
Integrante	Responsabilidade
Bernardo	Board + Random
JG	Fleet + Outro módulo
Guilherme	Game / I/O / integração final