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

    while(fscanf(arquivo, "%49[^;]; %29[^;]; % f; %d; %d\n",
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