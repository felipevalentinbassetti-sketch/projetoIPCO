#include <stdio.h>
#include "loja.h"
int main(){
    Jogo inventario[100];
    int totalJogos;
    int opcao;

    totalJogos = carregarJogos(inventario);

    printf("%d",totalJogos);
}