#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "minesweeper.h"

#define BLUE "\033[44m   \033[0m"  

Celula **criarTabuleiro(int linhas, int colunas) {
    Celula **tabuleiro = (Celula **)malloc(linhas * sizeof(Celula*));
    for (int i = 0; i < linhas; i++) {
        tabuleiro[i] = (Celula *)calloc(colunas, sizeof(Celula));
    }
    return tabuleiro;
}

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

void calcularVizinhos(Celula **tabuleiro, int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (!tabuleiro[i][j].mina) {
                int count = 0;
                for (int di = -1; di <= 1; di++) {
                    for (int dj = -1; dj <= 1; dj++) {
                        int ni = i + di, nj = j + dj;
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

void exibirTabuleiro(Celula **tabuleiro, int linhas, int colunas) {
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
    
    printf("    ");
    for (int j = 0; j < colunas; j++) {
        int num = j + 1;
        int ones = num % 10;
        printf("  %d ", ones);
    }
    printf("\n");
    
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

void jogar(Celula **tabuleiro, int linhas, int colunas, int num_minas) {
    char comando[10];
    int i, j;
    
    while (1) {
        exibirTabuleiro(tabuleiro, linhas, colunas);
        printf("Digite sua jogada: ");
        scanf("%s", comando);
        
        if (comando[0] == '#') {
            i = comando[1] - 'A';
            j = atoi(&comando[2]) - 1;
            tabuleiro[i][j].marcado = !tabuleiro[i][j].marcado;
        } else {
            i = comando[0] - 'A';
            j = atoi(&comando[1]) - 1;
            
            if (tabuleiro[i][j].mina) {
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
            printf("Parabéns! Você venceu!\n");
            return;
        }
    }
}

void liberarTabuleiro(Celula **tabuleiro, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(tabuleiro[i]);
    }
    free(tabuleiro);
}
