#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "btree.h"
#include "registeropertions.h"
#include "funcoesAuxiliares.h"

int byteoffset_bHeader(int rrn){
    return rrn*205+205;
}

btree_header criarArvoreB(){
    btree_header bHeader;
    bHeader.noRaiz = -1;
    bHeader.RRNproxNo = 0;
    bHeader.status = '0';
    return bHeader;
}

// Funcao para escrever cabecalho no arquivo de indices
void escreve_btree_header(FILE *indice, btree_header bHeader){
    fseek(indice, 0, SEEK_SET);
    fwrite(&(bHeader.status), sizeof(char), 1, indice);
    fwrite(&(bHeader.noRaiz), sizeof(int), 1, indice);
    fwrite(&(bHeader.RRNproxNo), sizeof(int), 1, indice);
    fwrite(LIXO, sizeof(char), 196, indice);
}


// Funcao para escrever nó no arquivo de indices
void escreve_node(FILE *indice, Node no, btree_header bHeader){
    fseek(indice, byteoffset_bHeader(bHeader.RRNproxNo), SEEK_SET);
    fwrite(&(no.nroChavesNo), sizeof(int), 1, indice);
    fwrite(&(no.alturaNo), sizeof(int), 1, indice);
    fwrite(&(no.RRNdoNO), sizeof(int), 1, indice);
    
    for (int i = 0; i < 3; i++) {
        fwrite(&(no.P[i]), sizeof(int), 1, indice);
        fwrite(no.C[i], strlen(no.C[i]) + 1, 1, indice); 
        fwrite(&(no.PR[i]), sizeof(int), 1, indice);
    }
    fwrite(&(no.P[3]), sizeof(int), 1, indice); 
}


// Funcao para escrever nó no arquivo de indices
void printNode(Node no){
    printf("%d, ", no.nroChavesNo);
    printf("%d, ", no.alturaNo);
    printf("%d, ", no.RRNdoNO);
    
    for (int i = 0; i < 3; i++) {
        printf("%d, ", no.P[i]);
        printf("%s, ", no.C[i]);
        printf("%d, ", no.PR[i]);

    }
    printf("%d, ", no.P[3]);
}

btree_header LerHeader(FILE* indice){
    btree_header bHeader;
    fseek(indice, 0, SEEK_SET);
    fread(&(bHeader.status), sizeof(char), 1, indice);
    fread(&(bHeader.noRaiz), sizeof(int), 1, indice);
    fread(&(bHeader.RRNproxNo), sizeof(int), 1, indice);
    return bHeader;
}


void read_node(FILE* indice, Node *no, int RRN){
    long pos = byteoffset_bHeader(RRN);
    fseek(indice, pos, SEEK_SET);

    // Lê o número real de chaves no nó
    fread(&(no->nroChavesNo), sizeof(int), 1, indice);

    // Lê outros campos,
    fread(&(no->alturaNo), sizeof(int), 1, indice);
    fread(&(no->RRNdoNO), sizeof(int), 1, indice);

    // Lê chaves, ponteiros e campos de referência
    for (int i = 0; i < 3; i++) {
        fread(&(no->P[i]), sizeof(int), 1, indice);
        fread(no->C[i], sizeof(char), 55, indice);
        removeLixo(no->C[i], strlen(no->C[i]));
        fread(&(no->PR[i]), sizeof(int), 1, indice);
    }

    // Lê os ponteiros restantes
    fread(&(no->P[3]), sizeof(int), 1, indice);
    
}

int Busca(FILE* indice, char *chave, int RRN) {
    Node no;
    read_node(indice, &no, RRN); // Implemente a função de leitura do nó do disco
    int i = 0;
    while (i < no.nroChavesNo && strcmp(chave, no.C[i]) > 0) {
        i++;
    }
    if (i < no.nroChavesNo && strcmp(chave, no.C[i]) == 0) {
        // A chave está presente na árvore
        return 1;
    }
    int cmp = strcmp(chave, no.C[i]);
    if (strlen(no.C[i]) == 0){
        cmp = -1;
    }
    if (no.P[i] != -1 && cmp < 0) {
        Busca(indice, chave, no.P[i]);
    } else if (no.P[i + 1] != -1) {
        Busca(indice, chave, no.P[i+1]);
    } else {
        // A chave não está presente na árvore
        return 0;
    }
}
