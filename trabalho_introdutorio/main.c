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
#include "binaryoperations.h"
#include "registeropertions.h"
#include "funcoesFornecidas.h"
#include "funcoesAuxiliares.h"
#include "btree.h"

int main(){
    int comando;
<<<<<<< Updated upstream
    /*scanf("%d", &comando);
=======
    scanf("%d", &comando);
>>>>>>> Stashed changes
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
    default:
        
        printf("Comando inexistente.\n");
<<<<<<< Updated upstream
    }*/
    Node no;
    FILE* arquivo = abrirArquivoLeitura("indice1.bin");
    btree_header bHeader = LerHeader(arquivo);
    Busca(arquivo, ".NETASP.NET", bHeader.noRaiz);
=======
    }

>>>>>>> Stashed changes


    return 0;
}
