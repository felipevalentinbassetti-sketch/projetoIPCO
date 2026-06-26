#include <stdio.h>
#include <string.h>
#include "../bib/loja.h"

int carregarJogos(Jogo vetorJogos[]){
    FILE *arquivo = fopen("txt/jogos.txt","r");
    int contador = 0;

    if(arquivo == NULL){
        printf("Arquivo 'txt/jogos.txt' nao existe, inventario vazio");
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
    FILE *arquivo = fopen("txt/jogos.txt", "w");
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
    
    FILE *arquivoCompras = fopen("txt/compras.txt", "a");
    if (arquivoCompras == NULL) {
        printf("Erro ao abrir o ficheiro 'txt/compras.txt' para registo.\n");
        return;
    }
    
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

    FILE *arquivoCompras = fopen("txt/compras.txt", "r");
    float faturamentoBruto = 0;
    int qtdTotalVendida = 0;
    
    if (arquivoCompras != NULL) {
        char nomeTemp[50];
        float valorTemp;
        int qtdTemp;
        
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
    printf("- Jogo mais vendido: %s (%d downloads)\n", vetorJogos[idxMaisVendido].nome, vetorJogos[idxMaisVendido].downloads);
    printf("- Quantidade total de downloads: %d\n", totalDownloadsMemoria);
    printf("- Faturamento bruto: R$ %.2f\n", faturamentoBruto);
    printf("- Impostos recolhidos (20%%): R$ %.2f\n", faturamentoBruto * 0.20);
    printf("- Faturamento liquido: R$ %.2f\n", faturamentoLiquido);
    printf("- Media de valor dos jogos disponiveis: R$ %.2f\n", mediaDisponiveis);
    printf("- Media de valor dos jogos vendidos: R$ %.2f\n", mediaVendidos);
    printf("- Jogo mais caro: %s (R$ %.2f)\n", vetorJogos[idxMaisCaro].nome, vetorJogos[idxMaisCaro].preco);
    printf("- Jogo mais barato: %s (R$ %.2f)\n", vetorJogos[idxMaisBarato].nome, vetorJogos[idxMaisBarato].preco);
    printf("========================================\n");
}

void gerenciarAvaliacoes(Jogo vetorJogos[], int totalJogos, int matrizAvaliacoes[][100], int qtdAvaliacoesPorJogo[]) {
    if (totalJogos == 0) {
        printf("\nNenhum jogo cadastrado para avaliacao.\n");
        return;
    }

    int opcao, escolha, nota;

    printf("\n--- SISTEMA DE AVALIACOES ---\n");
    printf("1. Registar nova avaliacao\n");
    printf("2. Exibir estatisticas de avaliacoes\n");
    printf("Escolha uma opcao: ");
    if (scanf("%d", &opcao) != 1) {
        printf("Opcao invalida.\n");
        return;
    }

    if (opcao == 1) {
        printf("\nSelecione um jogo para avaliar:\n");
        for (int i = 0; i < totalJogos; i++) {
            printf("%d. %s\n", i + 1, vetorJogos[i].nome);
        }
        printf("Opcao (1 a %d): ", totalJogos);
        if (scanf("%d", &escolha) != 1) {
            return;
        }
        
        int idx = escolha - 1;
        if (idx >= 0 && idx < totalJogos) {
            if (qtdAvaliacoesPorJogo[idx] >= 100) {
                printf("Erro: Limite maximo de 100 avaliacoes atingido para este jogo.\n");
                return;
            }
            do {
                printf("Atribua uma nota de 0 a 10 para '%s': ", vetorJogos[idx].nome);
                if (scanf("%d", &nota) != 1) {
                    printf("Entrada invalida! Introduza um numero.\n");
                    nota = -1;
                } else if (nota < 0 || nota > 10) {
                    printf("Nota invalida! Deve ser entre 0 e 10.\n");
                }
            } while (nota < 0 || nota > 10);
            
            int col = qtdAvaliacoesPorJogo[idx];
            matrizAvaliacoes[idx][col] = nota;
            qtdAvaliacoesPorJogo[idx]++;
            printf("Avaliacao registada com sucesso!\n");
        } else {
            printf("Jogo inexistente.\n");
        }
    } 
    else if (opcao == 2) {
        int melhorJogoIdx = -1;
        float maiorMedia = -1;
        int totalAvaliacoesSistema = 0;
        float somaGeralNotas = 0;

        for (int i = 0; i < totalJogos; i++) {
            if (qtdAvaliacoesPorJogo[i] > 0) {
                int somaNotasJogo = 0;
                for (int j = 0; j < qtdAvaliacoesPorJogo[i]; j++) {
                    somaNotasJogo += matrizAvaliacoes[i][j];
                    totalAvaliacoesSistema++;
                }
                
                float mediaJogo = (float)somaNotasJogo / qtdAvaliacoesPorJogo[i];
                somaGeralNotas += somaNotasJogo;

                if (mediaJogo > maiorMedia) {
                    maiorMedia = mediaJogo;
                    melhorJogoIdx = i;
                }
            }
        }

        if (totalAvaliacoesSistema == 0) {
            printf("\nNenhuma avaliacao registada no sistema ate ao momento.\n");
        } else {
            printf("\n========================================\n");
            printf("       ESTATISTICAS DE AVALIACOES\n");
            printf("========================================\n");
            printf("-Avaliacoes globais registadas: %d\n", totalAvaliacoesSistema);
            printf("-Media geral das avaliacoes: %.2f / 10\n", somaGeralNotas / totalAvaliacoesSistema);
            if (melhorJogoIdx != -1) {
                printf("-Jogo com melhor media: %s (Media: %.2f)\n", vetorJogos[melhorJogoIdx].nome, maiorMedia);
            }
            printf("========================================\n");
        }
    } else {
        printf("Opcao invalida.\n");
    }
}
