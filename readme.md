# ⚓ Batalha Naval (Battleship)

[![C Language](https://img.shields.io/badge/Language-C_11-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Este projeto implementa o clássico jogo Batalha Naval em um ambiente de linha de comando (CLI), focado na **modularização**, **gerenciamento explícito de memória**, e **programação estruturada em C**.

---

## 🎯 Objetivo e Funcionalidades

O projeto validou o domínio de conceitos essenciais de C, como `structs`, ponteiros, e alocação dinâmica (`malloc`/`realloc`/`free`).

* **Tabuleiro:** Configurável (padrão 10x10).
* **Frota:** Implementação da frota padrão (1x5, 1x4, 2x3, 2x2).
* **Visualização:** Símbolos de largura consistente para alinhamento CLI:
    * `X`: Acerto.
    * `O`: Navio Afundado (no mapa de tiros do atacante).
    * `#`: Navio Afundado (no tabuleiro de navios do defensor).
    * `.`: Tiro na Água (Miss).

---

## 🛠️ Decisões de Design e Arquitetura

O projeto seguiu o princípio da **separação de responsabilidades** e focou na gestão manual de memória.

### 1. Gerenciamento de Memória

* **Alocação Dinâmica:** O `Board` (tabuleiro) e a `Fleet` (frota) são alocados e dimensionados usando `malloc` e `realloc`.
* **`free_game_memory()`:** Garante que toda a memória alocada (`b->cells`, `f->ships`) seja liberada (`free`) ao final do jogo, prevenindo *memory leaks*.

### 2. Lógica de Combate

* **Rastreamento com `ship_id`:** O `ship_id` em cada célula permite que a função `fire_shot` rastreie o navio exato na `Fleet` para atualizar o contador de `hits`.
* **`fire_shot` (Dupla Marcação):** Quando um navio afunda, a função `mark_ship_sunk` é chamada duas vezes: uma para o tabuleiro real (`target_board`) e outra para o mapa de tiros do atacante (`shots` board), garantindo a atualização visual de `X` para `O` em ambos os lados.

---

## 📁 Estrutura do Código

| Módulo | Arquivos | Responsabilidade Principal |
| :--- | :--- | :--- |
| **Núcleo** | `main.c`, `game.c`, `game.h` | Fluxo de turnos, regras e estado global do jogo. |
| **Tabuleiro** | `board.c`, `board.h` | Definição da estrutura `Board`, validação e posicionamento. |
| **Frota** | `fleet.c`, `fleet.h` | Gerenciamento de navios, controle de acertos e afundamentos. |
| **I/O** | `io.c`, `io.h` | Leitura de coordenadas, impressão formatada. |

---

## 🚀 Compilação e Execução

### Pré-requisitos

Você precisa ter o **GCC** (GNU Compiler Collection) e o **GNU Make** instalados.

### 1. Compilação Padrão (Recomendado)

Navegue até o diretório raiz e use o `make`:

```bash
make
```

### 2. Fallback: Compilação Manual (Se 'make' falhar)

Se o make não funcionar, execute o comando gcc completo para linkar todos os arquivos:

```bash
gcc -std=c11 -Wall -Wextra -pedantic main.c game.c board.c fleet.c io.c rnd.c -o battleship
```
### 3. Execução

``` Bash
./battleship
```

## 👥 Equipe
```md
* João Guilherme Grimaldi 
* Guilherme Agra
* Bernardo Pedrosa   
```
