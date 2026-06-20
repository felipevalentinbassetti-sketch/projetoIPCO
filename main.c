#include <stdio.h>
#include "bib/loja.h"
int main(){
    Jogo inventario[100];
    int totalJogos;
    int opcao;

    totalJogos = carregarJogos(inventario);

    printf("%d",totalJogos);

    int matrizAvaliacoes[100][100]; 
    int qtdAvaliacoesPorJogo[100] = {0}; 


    do {
        printf("\n========================================\n");
        printf("       MENU - LOJA DE GAMES DIGITAIS    \n");
        printf("========================================\n");
        printf("1. Cadastrar novo jogo\n");
        printf("2. Registar compra/download\n");
        printf("3. Relatorios Financeiros e de Vendas\n");
        printf("4. Avaliacoes dos Jogos\n");
        printf("0. Sair e Salvar\n");
        printf("========================================\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("\nErro: Entrada invalida. A encerrar de forma segura...\n");
            break;
        }

        switch(opcao) {
            case 1:
                totalJogos = cadastrarJogo(inventario, totalJogos);
                break;
            case 2:
                registrarCompra(inventario, totalJogos);
                break;
            case 3:
                exibirRelatorios(inventario,totalJogos);
                break;
            case 4:
                gerenciarAvaliacoes(inventario, totalJogos, matrizAvaliacoes, qtdAvaliacoesPorJogo);
                break;
            case 0:
                salvarJogos(inventario, totalJogos); 
                printf("\nDados guardados em 'jogos.txt'. Obrigado por utilizar o sistema!\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while(opcao != 0);

    return 0;
}