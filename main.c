/*
 * Trabalho Final - Minesweeper
 * Alunos: Ravena Marques Carvalho e Raissa Costa Sousa
 * Disciplina: Programação (CK0226)
 * Professor: Miguel Franklin
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "minesweeper.h"

int main() {
    int linhas, colunas, num_minas;
    srand(time(NULL));  // Inicializa a semente

    // Lê dimensões do tabuleiro
    printf("Digite o número de linhas (5-26): ");
    scanf("%d", &linhas);
    printf("Digite o número de colunas (5-40): ");
    scanf("%d", &colunas);
    
    // Valida dimensões
    if (linhas < MIN_LINHAS || linhas > MAX_LINHAS || colunas < MIN_COLUNAS || colunas > MAX_COLUNAS) {
        printf("Dimensão inválida!\n");
        return 1;
    }
    
    int max_minas = (linhas * colunas) / 5;
    // Lê número de minas
    printf("Digite o número de minas (7-%d): ", max_minas);
    scanf("%d", &num_minas);
    
    // Valida número de minas
    if (num_minas < MIN_MINAS || num_minas > max_minas) {
        printf("Número de minas inválido!\n");
        return 1;
    }
    
    // Cria e inicializa o tabuleiro
    Celula **tabuleiro = criarTabuleiro(linhas, colunas);
    posicionarMinas(tabuleiro, linhas, colunas, num_minas);
    calcularVizinhos(tabuleiro, linhas, colunas);
    
    // Inicia o jogo e libera memória
    jogar(tabuleiro, linhas, colunas, num_minas);
    liberarTabuleiro(tabuleiro, linhas);
    
    return 0;
}
