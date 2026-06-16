#include <stdio.h>
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
        printf("Erro feio.\n");
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

void registrarCompra(Jogo vetorJogos[], int totalJogos) {
    if (totalJogos == 0) {
        printf("\nNenhum jogo cadastrado no sistema ainda.\n");
        return;
    }

    int escolha, quantidade;
    
    printf("\n--- REGISTAR COMPRA/DOWNLOAD ---\n");
    printf("Jogos disponiveis:\n");
    for (int i = 0; i < totalJogos; i++) {
        printf("%d. %s (R$ %.2f) - Downloads atuais: %d\n", i + 1, vetorJogos[i].nome, vetorJogos[i].preco, vetorJogos[i].downloads);
    }
    
    printf("\nEscolha o numero do jogo (1 a %d): ", totalJogos);
    if (scanf("%d", &escolha) != 1 || escolha < 1 || escolha > totalJogos) {
        printf("Opcao invalida!\n");
        return;
    }
    
    int indice = escolha - 1;
    
    printf("Quantidade de copias/downloads: ");
    if (scanf("%d", &quantidade) != 1 || quantidade <= 0) {
        printf("Quantidade invalida! Operacao cancelada.\n");
        return;
    }
    
    vetorJogos[indice].downloads += quantidade;
    
    FILE *arquivoCompras = fopen("compras.txt", "a");
    if (arquivoCompras == NULL) {
        printf("Erro ao abrir o ficheiro 'compras.txt' para registo.\n");
        return;
    }
    
    // Gravacao inclui os pontos e virgulas corretamente
    fprintf(arquivoCompras, "%s; %.2f; %d\n", vetorJogos[indice].nome, vetorJogos[indice].preco, quantidade);
    fclose(arquivoCompras);
    
    float totalGasto = vetorJogos[indice].preco * quantidade;
    printf("\n>>> Compra registada com sucesso! <<<\n");
    printf("Jogo: %s | Qtd: %d | Total Bruto: R$ %.2f\n", vetorJogos[indice].nome, quantidade, totalGasto);
}


void exibirRelatorios(Jogo vetorJogos[], int totalJogos) {
    if (totalJogos == 0) {
        printf("\nNenhum jogo cadastrado no sistema para gerar relatorios.\n");
        return;
    }

    int idxMaisVendido = 0, idxMaisCaro = 0, idxMaisBarato = 0;
    int totalDownloadsMemoria = 0;
    float somaPrecosDisponiveis = 0;

    for (int i = 0; i < totalJogos; i++) {
        totalDownloadsMemoria += vetorJogos[i].downloads;
        somaPrecosDisponiveis += vetorJogos[i].preco;

        if (vetorJogos[i].downloads > vetorJogos[idxMaisVendido].downloads) {
            idxMaisVendido = i;
        }
        if (vetorJogos[i].preco > vetorJogos[idxMaisCaro].preco) {
            idxMaisCaro = i;
        }
        if (vetorJogos[i].preco < vetorJogos[idxMaisBarato].preco) {
            idxMaisBarato = i;
        }
    }

    FILE *arquivoCompras = fopen("compras.txt", "r");
    float faturamentoBruto = 0;
    int qtdTotalVendida = 0;
    
    if (arquivoCompras != NULL) {
        char nomeTemp[50];
        float valorTemp;
        int qtdTemp;
        
        // BUG CORRIGIDO: Adicionado o ';' apos o %f que faltava no padrao de leitura!
        while (fscanf(arquivoCompras, " %49[^;]; %f; %d\n", nomeTemp, &valorTemp, &qtdTemp) == 3) {
            faturamentoBruto += (valorTemp * qtdTemp);
            qtdTotalVendida += qtdTemp;
        }
        fclose(arquivoCompras);
    }

    float faturamentoLiquido = faturamentoBruto * 0.80; // Retem 80% apos imposto
    float mediaDisponiveis = somaPrecosDisponiveis / totalJogos;
    float mediaVendidos = (qtdTotalVendida > 0) ? (faturamentoBruto / qtdTotalVendida) : 0;

    printf("\n========================================\n");
    printf("         RELATORIOS DO SISTEMA\n");
    printf("========================================\n");
    printf("â€¢ Jogo mais vendido: %s (%d downloads)\n", vetorJogos[idxMaisVendido].nome, vetorJogos[idxMaisVendido].downloads);
    printf("â€¢ Quantidade total de downloads: %d\n", totalDownloadsMemoria);
    printf("â€¢ Faturamento bruto: R$ %.2f\n", faturamentoBruto);
    printf("â€¢ Impostos recolhidos (20%%): R$ %.2f\n", faturamentoBruto * 0.20);
    printf("â€¢ Faturamento liquido: R$ %.2f\n", faturamentoLiquido);
    printf("â€¢ Media de valor dos jogos disponiveis: R$ %.2f\n", mediaDisponiveis);
    printf("â€¢ Media de valor dos jogos vendidos: R$ %.2f\n", mediaVendidos);
    printf("â€¢ Jogo mais caro: %s (R$ %.2f)\n", vetorJogos[idxMaisCaro].nome, vetorJogos[idxMaisCaro].preco);
    printf("â€¢ Jogo mais barato: %s (R$ %.2f)\n", vetorJogos[idxMaisBarato].nome, vetorJogos[idxMaisBarato].preco);
    printf("========================================\n");
}
