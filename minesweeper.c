#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include "minesweeper.h"

#define BLUE "\033[44m   \033[0m"  

// Cria o tabuleiro alocando memória para as células
Celula **criarTabuleiro(int linhas, int colunas) {
    Celula **tabuleiro = (Celula **)malloc(linhas * sizeof(Celula *));
    for (int i = 0; i < linhas; i++) {
        tabuleiro[i] = (Celula *)calloc(colunas, sizeof(Celula));
    }
    return tabuleiro;
}

// Posiciona as minas de forma aleatória
void posicionarMinas(Celula **tabuleiro, int linhas, int colunas, int num_minas) {
    int colocadas = 0;
    while (colocadas < num_minas) {
        int r_linha = rand() % linhas;
        int r_coluna = rand() % colunas;
        if (!tabuleiro[r_linha][r_coluna].mina) {
            tabuleiro[r_linha][r_coluna].mina = 1;
            colocadas++;
        }
    }
}

// Calcula o número de minas vizinhas para cada célula
void calcularVizinhos(Celula **tabuleiro, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (!tabuleiro[i][j].mina) {
                int count = 0;
                for (int di = -1; di <= 1; di++) {
                    for (int dj = -1; dj <= 1; dj++) {
                        int ni = i + di;
                        int nj = j + dj;
                        if (ni >= 0 && ni < linhas && nj >= 0 && nj < colunas &&
                            tabuleiro[ni][nj].mina) {
                            count++;
                        }
                    }
                }
                tabuleiro[i][j].vizinhos = count;
            }
        }
    }
}

// Exibe o tabuleiro com as coordenadas
void exibirTabuleiro(Celula **tabuleiro, int linhas, int colunas) {
    // Exibe a numeração das colunas (dezenas)
    printf("    ");
    for (int j = 0; j < colunas; j++) {
        int num = j + 1;
        if (num >= 10) {
            int tens = num / 10;
            printf("  %d ", tens);
        } else {
            printf("    ");
        }
    }
    printf("\n");

    // Exibe a numeração das colunas do tabuleiro
    printf("    ");
    for (int j = 0; j < colunas; j++) {
        int num = j + 1;
        int ones = num % 10;
        printf("  %d ", ones);
    }
    printf("\n");

    // Exibe as linhas do tabuleiro
    for (int i = 0; i < linhas; i++) {
        printf("   ");
        for (int j = 0; j < colunas; j++) {
            printf("+---");
        }
        printf("+\n");

        printf(" %c ", 'A' + i);
        for (int j = 0; j < colunas; j++) {
            if (tabuleiro[i][j].revelado) {
                if (tabuleiro[i][j].mina) {
                    printf("| * ");
                } else {
                    printf("| %2d", tabuleiro[i][j].vizinhos);
                }
            } else if (tabuleiro[i][j].marcado) {
                printf("| # ");
            } else {
                printf("|%s", BLUE);
            }
        }
        printf("|\n");
    }
    printf("   ");
    for (int j = 0; j < colunas; j++) {
        printf("+---");
    }
    printf("+\n");
}

// Abre uma célula e, se ela não possuir minas vizinhas, expande recursivamente
void abrirCelula(Celula **tabuleiro, int linhas, int colunas, int i, int j) {
    if (i < 0 || i >= linhas || j < 0 || j >= colunas || tabuleiro[i][j].revelado)
        return;
    
    tabuleiro[i][j].revelado = 1;
    
    if (tabuleiro[i][j].vizinhos == 0) {
        for (int di = -1; di <= 1; di++) {
            for (int dj = -1; dj <= 1; dj++) {
                int ni = i + di;
                int nj = j + dj;
                if (ni >= 0 && ni < linhas && nj >= 0 && nj < colunas) {
                    abrirCelula(tabuleiro, linhas, colunas, ni, nj);
                }
            }
        }
    }
}

// Revela todas as minas no tabuleiro (usada quando o jogador acerta uma mina)
void revelarMinas(Celula **tabuleiro, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            tabuleiro[i][j].revelado = 1;
        }
    }
}

// Função principal do jogo
void jogar(Celula **tabuleiro, int linhas, int colunas, int num_minas) {
    char comando[10];
    int i, j;
    char tipo_comando; 

    while (1) {
        exibirTabuleiro(tabuleiro, linhas, colunas);

        int entrada_valida = 0;
        while (!entrada_valida) {
            printf("Digite sua jogada: ");
            scanf("%s", comando);

            tipo_comando = comando[0]; 
            int offset = (tipo_comando == '#' || tipo_comando == '!') ? 1 : 0; 

            if (isalpha(comando[offset])) {
                i = toupper(comando[offset]) - 'A';

                if (isdigit(comando[offset + 1])) {
                    j = atoi(&comando[offset + 1]) - 1;
                } else {
                    printf("Entrada inválida! Use o formato correto (ex: A5, #B7, !C3).\n");
                    continue;
                }
            } else {
                printf("Entrada inválida! Use o formato correto (ex: A5, #B7, !C3).\n");
                continue;
            }

            if (i >= 0 && i < linhas && j >= 0 && j < colunas) {
                entrada_valida = 1;
            } else {
                printf("Coordenada fora dos limites! Tente novamente.\n");
            }
        }

        if (tipo_comando == '#') { 
            if (!tabuleiro[i][j].revelado) {
                tabuleiro[i][j].marcado = 1;
            }
        } else if (tipo_comando == '!') {  
            if (tabuleiro[i][j].marcado) {
                tabuleiro[i][j].marcado = 0;
            }
        } else { 
            if (tabuleiro[i][j].mina) {
                printf("Você acertou uma mina!\n");
                revelarMinas(tabuleiro, linhas, colunas);
                exibirTabuleiro(tabuleiro, linhas, colunas);
                printf("Você acertou uma mina! Fim de jogo.\n");
                return;
            }
            abrirCelula(tabuleiro, linhas, colunas, i, j);
        }

        int celulas_reveladas = 0, total_celulas = linhas * colunas - num_minas;
        for (int x = 0; x < linhas; x++) {
            for (int y = 0; y < colunas; y++) {
                if (tabuleiro[x][y].revelado) {
                    celulas_reveladas++;
                }
            }
        }
        if (celulas_reveladas == total_celulas) {
            exibirTabuleiro(tabuleiro, linhas, colunas);
            printf("Parabéns! Você venceu!\n");
            return;
        }
    }
}

// Libera a memória alocada para o tabuleiro
void liberarTabuleiro(Celula **tabuleiro, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(tabuleiro[i]);
    }
    free(tabuleiro);
}

