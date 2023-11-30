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

Node criaNode(){
    Node no;
    no.nroChavesNo = 0;
    no.alturaNo = 1;
    no.RRNdoNO = 0;
    for (int i = 0; i < 3; i++) {
        no.P[i] = -1;
        no.C[i][0] = '\0';
        no.PR[i] = -1;
    }
    no.P[3] = -1;
    return no;
}
// Funcao para escrever cabecalho no arquivo de indices
void escreve_btree_header(FILE *indice, btree_header *bHeader){
    fseek(indice, 0, SEEK_SET);
    fwrite(&(bHeader->status), sizeof(char), 1, indice);
    fwrite(&(bHeader->noRaiz), sizeof(int), 1, indice);
    fwrite(&(bHeader->RRNproxNo), sizeof(int), 1, indice);

    int aux = 196;
    
    char trash[aux];

    for(int i=0; i<aux; i++){
        trash[i] = LIXO;
    }

    fwrite(trash, sizeof(char), aux, indice);
}


// Funcao para escrever nó no arquivo de indices
void escreve_node(FILE *indice, Node *no){
    fseek(indice, byteoffset_bHeader(no->RRNdoNO), SEEK_SET);
    fwrite(&(no->nroChavesNo), sizeof(int), 1, indice);
    fwrite(&(no->alturaNo), sizeof(int), 1, indice);
    fwrite(&(no->RRNdoNO), sizeof(int), 1, indice);
    
    for (int i = 0; i < 3; i++) {
        fwrite(&(no->P[i]), sizeof(int), 1, indice);
        fwrite(no->C[i], strlen(no->C[i]), 1, indice); 
        int aux = 55-strlen(no->C[i]);
    
        char trash[aux];

        for(int i=0; i<aux; i++){
            trash[i] = LIXO;
        }

        fwrite(trash, sizeof(char), aux, indice);
            fwrite(&(no->PR[i]), sizeof(int), 1, indice);
        }
    fwrite(&(no->P[3]), sizeof(int), 1, indice); 
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
    read_node(indice, &no, RRN);
    int i = 0;
    while (i < no.nroChavesNo && strcmp(chave, no.C[i]) > 0) {
        i++;
    }

    if (strcmp(chave, no.C[i]) == 0) {
        // A chave está presente na árvore
        return no.PR[i];
    } 
    if (no.P[i] == -1){
        return -1; // Não há chave!!!
    }
    return Busca(indice, chave, no.P[i]);
}


Node BuscaNoFolha(FILE* indice, int RRN, char* chave){

    

    Node no;
    read_node(indice, &no, RRN); // Implemente a função de leitura do nó do disco

    if (no.alturaNo == 1){
        return no;
    }

    int i = 0;
    while (i < no.nroChavesNo && strcmp(chave, no.C[i]) > 0) {
        i++;
    }
    
    return BuscaNoFolha(indice, no.P[i], chave);
}


int insereNoIndice(Node *no, char* newKey, int PR, int P){
    int pos = 0;

    while (pos < 3) { 
        if (strlen(no->C[pos]) == 0){ // Se a posição nao tiver nada, insere a chave
            strcpy(no->C[pos], newKey);
            no->PR[pos] = PR;
            no->P[pos + 1] = P;
            break;
        } else if (strcmp(newKey, no->C[pos]) < 0){ // Caso contrario, ve se a chave deveria estar na posicao da chave ja inserida
            no->P[3] = no->P[2]; // Se sim, desloca as demais chaves para frente
            strcpy(no->C[2], no->C[1]);
            no->PR[2] = no->PR[1];
            if (pos == 0){
                no->P[2] = no->P[1];
                no->P[1] = no->P[0];
                strcpy(no->C[1], no->C[0]);
                no->PR[1] = no->PR[0];
                strcpy(no->C[0], newKey);
                no->PR[0] = PR;
                no->P[pos + 1] = P;
            } else {
                strcpy(no->C[1], newKey);
                no->PR[1] = PR;
                no->P[pos + 1] = P;   
            }   
            break;      
        }
        pos++;
    }
    no->nroChavesNo++;
    return pos;

}

int BuscaPai(FILE* indice, int RRN, char* chave, int aux){
    Node no;
    read_node(indice, &no, RRN); // Implemente a função de leitura do nó do disco
    int i = 0;
    while (i < no.nroChavesNo && strcmp(chave, no.C[i]) > 0) {
        i++;
    }

    if (i < no.nroChavesNo && strcmp(chave, no.C[i]) == 0) {
        return aux;
    }


    return BuscaPai(indice, no.P[i], chave, no.RRNdoNO);
}

// Node no - No que vai ser splitado; Char* newKey - Chave que vai entrar no nó
btree_header InsereOverflow(FILE* indice, btree_header bHeader, Node* no, char* newKey, int PR, int P) {
    // Verifica se ta no nó raiz, se estiver, splita e cria outro nó
    int RRN_pai = BuscaPai(indice, bHeader.noRaiz, no->C[0], -1);
    Node pai;
    if (RRN_pai != -1){
        read_node(indice, &pai, RRN_pai);
    }

    // Com o nó pai, ordenar o no a ser splitado e alocar a chave na posição [1] no pai
    Node auxiliar = criaNode();
    int pos = 0;
    while (pos < no->nroChavesNo && strcmp(newKey, no->C[pos]) > 0) {
        pos++;
    }

    char temp[4][256];
    int PRtemp[4];
    int Ptemp[5];


    // Preenche os vetores temp com as chaves e seus respectivos PR e P
    for (int i = 0; i < 4; i++) {
        if (i != 3) {
            strcpy(temp[i], no->C[i]);
            PRtemp[i] = no->PR[i];
            Ptemp[i] = no->P[i];
        } else if (i == 3) {
            strcpy(temp[i], newKey);
            PRtemp[i] = PR;
            Ptemp[i] = no->P[i];  
            Ptemp[i + 1] = P;
        } 
    }

// Ordena o vetor
    for (int i = 3; i >= pos; i--) {
        if (i != pos){
            strcpy(temp[i], temp[i - 1]);
            PRtemp[i] = PRtemp[i - 1];
            Ptemp[i + 1] = Ptemp[i];
        } else {
            strcpy(temp[i], newKey);
            PRtemp[i] =  PR;
            Ptemp[i + 1] = P;
        }
    }
    // Cria novo nó, colocando as duas ultimas posicoes do nó no auxiliar
    auxiliar.P[0] = Ptemp[3];
    auxiliar.PR[0] = PRtemp[3];
    strcpy(auxiliar.C[0], temp[3]);
    auxiliar.P[1] =  Ptemp[4];
    auxiliar.alturaNo = no->alturaNo;
    auxiliar.nroChavesNo = 1;
    auxiliar.RRNdoNO = bHeader.RRNproxNo;
    bHeader.RRNproxNo++;

    // Apaga valores do no que foram retirados
    for (int i = 0; i < 3; i++) {
        no->P[i] = -1;
        no->P[i + 1] = -1;
        no->C[i][0] = '\0';
        no->PR[i] = -1;
    }

    // Atribui os valores ordenados
    no->P[0] = Ptemp[0];
    strcpy(no->C[0], temp[0]);
    no->PR[0] = PRtemp[0];
    no->P[1] = Ptemp[1];
    no->PR[1] = PRtemp[1];
    strcpy(no->C[1], temp[1]);
    no->P[2] = Ptemp[2];
    no->nroChavesNo = 2;


    // Fazer isso recursivamente até o nó tiver espaço ou for o nó raiz    
    // Verifica se está na raiz ou se o nó pai tem espaço para receber
    if (pai.nroChavesNo == 3 || RRN_pai == -1){
        if (RRN_pai != -1){
            // Se pai nao tem espaço, chama novamente a funcao, passando a posicao [1] como nova chave e o no auxiliar com ptr
            pai.alturaNo = no->alturaNo + 1;
            bHeader = InsereOverflow(indice, bHeader, &pai, temp[2], PRtemp[2], auxiliar.RRNdoNO);
        } else {
            // Se estiver no nó raiz, splita e cria novo nó. nó a direita vira auxiliar, a esquerda o que antes era a raiz
            Node raiz = criaNode();
            raiz.alturaNo = (no->alturaNo + 1);
            raiz.nroChavesNo = 1;
            raiz.RRNdoNO = bHeader.RRNproxNo;
            bHeader.RRNproxNo++;
            bHeader.noRaiz = raiz.RRNdoNO;
            strcpy(raiz.C[0], temp[2]);
            raiz.PR[0] = PRtemp[2];
            raiz.P[0] = no->RRNdoNO;
            raiz.P[1] = auxiliar.RRNdoNO;
            escreve_node(indice, &raiz);
        }
    } else {
        // Se pai tem espaço, insere nele
        insereNoIndice(&pai, temp[2], PRtemp[2], auxiliar.RRNdoNO);
        escreve_node(indice, &pai);
    }

    // Escreve os nós no arquivo de indices, e retorna o cabecalho para a funcao 
    
    escreve_node(indice, no);
    escreve_node(indice, &auxiliar);

    return bHeader;
}

btree_header InserirNo(FILE *indice, char* newKey, int PR){
    // 1. Procurar a folha onde a chave deve ser inserida
    Node no;
    btree_header bHeader = LerHeader(indice);
    if (bHeader.noRaiz != -1){
        no = BuscaNoFolha(indice, bHeader.noRaiz, newKey); // Procura o nó folha que a chave deveria ser inserido e atribui o nó a no
    } else {
        // Caso da árvore Vazia
        no = criaNode();
        bHeader.noRaiz = 0;
        bHeader.RRNproxNo++;
    }

    // 2. Inserir a chave na folha
    if (no.nroChavesNo != 3){
        insereNoIndice(&no, newKey, PR, -1);
        escreve_node(indice, &no);
    } else { // Overflow
        // split
        bHeader = InsereOverflow(indice, bHeader, &no, newKey, PR, -1);
    }
    escreve_btree_header(indice, &bHeader);
    return bHeader;
}