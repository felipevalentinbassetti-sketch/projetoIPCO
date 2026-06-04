#ifndef LOJA_H
#define LOJA_H

typedef struct{
    char nome[50];
    char genero[30];
    float preco;
    int classificacao;
    int downloads;
}Jogo;

int carregarJogos(Jogo vetorJogos[]);
#endif