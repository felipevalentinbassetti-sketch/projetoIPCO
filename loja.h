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
int cadastrarJogo(Jogo vetorJogos[], int totalJogos);
void salvarJogos(Jogo vetorJogos[], int totalJogos);
void registrarCompra(Jogo vetorJogos[], int totalJogos);
void exibirRelatorios(Jogo vetorJogos[], int totalJogos);

#endif