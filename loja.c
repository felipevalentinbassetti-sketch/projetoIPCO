#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "loja.h"

int carregarJogos(Jogo vetorJogos[]){
    FILE *arquivo = fopen("jogos.txt","r");
    int contador = 0;

    if(arquivo == NULL){
        printf("Arquivo 'jogos.txt' nao existe, inventario vazio");
        return 0;
    }

    while(fscanf(arquivo, "%49[^;]; %29[^;]; %f; %d; %d\n",
        vetorJogos[contador].nome,
        vetorJogos[contador].genero,
        &vetorJogos[contador].preco,
        &vetorJogos[contador].classificacao,
        &vetorJogos[contador].downloads)==5){
            contador++;
            if(contador>=100){
                printf("Limite maximo de jogos atingidos\n");
                break;
            }
        }

    fclose(arquivo);
    printf("Sucesso: %d jogo(s) carregado(s)\n",contador);
    return contador;
}

int cadastrarJogo(Jogo vetorJogos[], int totalJogos){
    if(totalJogos >= 100){
        printf("Limite maximo de jogos atingidos\n");
        return totalJogos;
    }

    printf("\n--- CADASTRO DE NOVO JOGO ---\n");
    
    printf("Nome do jogo: ");
    scanf(" %49[^\n]", vetorJogos[totalJogos].nome);
    
    printf("Genero: ");
    scanf(" %29[^\n]", vetorJogos[totalJogos].genero);
    
    do {
        printf("Preco (R$): ");
        if (scanf("%f", &vetorJogos[totalJogos].preco) != 1) {
            printf("Erro: Formato invalido. Introduza um valor numerico.\n");
            
            vetorJogos[totalJogos].preco = -1; 
        } else if (vetorJogos[totalJogos].preco < 0) {
            printf("Erro: O preco nao pode ser negativo. Tente novamente.\n");
        }
    } while (vetorJogos[totalJogos].preco < 0);
    
    do {
        printf("Classificacao indicativa (idade minima): ");
        if (scanf("%d", &vetorJogos[totalJogos].classificacao) != 1) {
            printf("Erro: Introduza um numero inteiro.\n");
            
            vetorJogos[totalJogos].classificacao = -1;
        }
    } while(vetorJogos[totalJogos].classificacao < 0);
    
    do {
        printf("Quantidade inicial de downloads/vendas: ");
        if (scanf("%d", &vetorJogos[totalJogos].downloads) != 1) {
            printf("Erro: Introduza um numero inteiro.\n");
            
            vetorJogos[totalJogos].downloads = -1;
        }
    } while (vetorJogos[totalJogos].downloads < 0);
    
    printf("\nJogo registado com sucesso na memoria!\n");
    return totalJogos + 1;
}

void salvarJogos(Jogo vetorJogos[], int totalJogos) {
    FILE *arquivo = fopen("jogos.txt", "w");
    if (arquivo == NULL) {
        printf("Erro critico: Nao foi possivel abrir o ficheiro 'jogos.txt' para escrita.\n");
        return;
    }
    
    for (int i = 0; i < totalJogos; i++) {
        fprintf(arquivo, "%s; %s; %.2f; %d; %d\n", 
                vetorJogos[i].nome, 
                vetorJogos[i].genero, 
                vetorJogos[i].preco, 
                vetorJogos[i].classificacao, 
                vetorJogos[i].downloads);
    }
    fclose(arquivo);
}