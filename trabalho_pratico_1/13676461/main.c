/*
* Name: Lucas Corlete Alves de Melo - NUSP: 13676461; Jean Carlos Pereira Cassiano - NUSP: 138640008
* Course: SCC0607 - Estrutura de Dados III
* Professor: Cristina Dutra de Aguiar
* Project: Trabalho Introdutório, 1 e 2 de ED3
* Description: Este trabalho tem como objetivo armazenar dados em um arquivo binário bem como desenvolver funcionalidades para a manipulação desses dados. Novas funcionalidades serão adicionadas conforme o avançar da disciplina.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "btree.h"
#include "binaryoperations.h"
#include "registeropertions.h"
#include "funcoesFornecidas.h"
#include "funcoesAuxiliares.h"


int main(){
    int comando;
    scanf("%d", &comando);

    switch (comando)
    {
    case 1:
        criaTable();
        break;
    case 2:
        LerBIN();
        break;
    case 3:
        selectWhere();
        break;
    case 4:
        buscarRRN();
        break;
    case 5:
        btreeCreateTable();
        break;
    default:
        printf("Comando inexistente.\n");
    }


    /*btree_header bHeader = criarArvoreB();
    FILE* indice = abrirArquivoEscrita("teste.bin");
    escreve_btree_header(indice, &bHeader);
    int i = 0;
    char temp[51];
    while (i != -1){
        scanf("%d", &i);
        if (i == -1){
            break;
        }
        scanf("%s", &temp);
        InserirNo(indice, &temp, i);
    }
    fclose(indice);*/
    
    return 0;
}
