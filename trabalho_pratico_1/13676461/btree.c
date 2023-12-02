/*
* Name: Lucas Corlete Alves de Melo - NUSP: 13676461; Jean Carlos Pereira Cassiano - NUSP: 138640008
* Course: SCC0607 - Estrutura de Dados III
* Professor: Cristina Dutra de Aguiar
* Project: Trabalho Introdutório, 1 e 2 de ED3
* Description: Este trabalho tem como objetivo armazenar dados em um arquivo binário bem como desenvolver funcionalidades para a 
* manipulação desses dados. Novas funcionalidades serão adicionadas conforme o avançar da disciplina.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "btree.h"
#include "registeropertions.h"
#include "funcoesAuxiliares.h"

/**
 * @brief Calcula o byte offset para um registro da nossa arvore, dado um RRN 
 * 
 * @param rrn
 */
int byteoffset_bHeader(int rrn){
    return rrn*TAM_REGISTRO_B+TAM_REGISTRO_B;
}

/**
 * @brief "Cria" nosssa arvore B, incializando o cabeçalho do nó com as devidas especificações
 * 
 * @return btree_header 
 */
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
        no.C[i][0] = NULL_TERM;
        no.PR[i] = -1;
    }
    no.P[3] = -1;
    return no;
}

/**
 * @brief Funcao para escrever cabecalho no arquivo de indices
 * 
 * @param indice 
 * @param bHeader 
 */
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


/**
 * @brief Funcao para escrever nó no arquivo de indices
 * 
 * @param indice 
 * @param no 
 */
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


/**
 * @brief Funcao para imprimir nó da arvore
 * 
 * @param no 
 */
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

/**
 * @brief Leitura do cabeçalho (arvore B)
 * 
 * @param indice 
 * @return btree_header 
 */
btree_header LerHeader(FILE* indice){
    btree_header bHeader;
    fseek(indice, 0, SEEK_SET);
    fread(&(bHeader.status), sizeof(char), 1, indice);
    fread(&(bHeader.noRaiz), sizeof(int), 1, indice);
    fread(&(bHeader.RRNproxNo), sizeof(int), 1, indice);
    return bHeader;
}

/**
 * @brief Implemente a função de leitura do nó do disco. Além de evitar a leitura de lixo.
 * 
 * @param indice 
 * @param no 
 * @param RRN 
 */
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

/**
 * @brief Busca sequencial recursiva para encontrar no, dada uma chave
 * 
 * @param indice 
 * @param chave 
 * @param RRN 
 * @return campo de referência para o registro no arquivo de dados, caso a chave esteja na arvore ou -1, caso não esteja.
 */
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


/**
 * @brief Busca (recursiva) de um no do tipo folha, dada uma chave
 * 
 * @param indice 
 * @param RRN 
 * @param chave 
 * @return Node 
 */
Node BuscaNoFolha(FILE* indice, Node no, char* chave){
    int i = 0;

    while (i < no.nroChavesNo && strcmp(chave, no.C[i]) > 0) {
        i++;
    }
    
    Node aux;
    read_node(indice, &aux, no.P[i]);
    return aux;
}

/**
 * @brief Realiza a inserção da nova chave no nó. Utiliza um loop para busca da posição correta: se estiver vazio, insere, 
 * se não, verifica se deve ser inserido antes ou depois de chave já existente. Após a inserção, atualiza a quantidade de chaves no no.
 * 
 * @param no 
 * @param newKey 
 * @param PR 
 * @param P 
 * @return posicao onde a chave deve ser inserida
 */
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

/**
 * @brief Split 2-1 que divide um nó durante a inserção. Quando o no está cheio, ocorre o split e as chaves são redistribuidas (balanceada).
 * Identifica a posição correta para inserir a nova chave no nó, mantendo a ordem das chaves.
 * Preenche arrays temporários com as chaves, valores de PR e ponteiros do nó. Em seguida ordena o vettor temporário.
 * Cria um novo nó auxiliar com as duas últimas posições do vetor temporário e
 * atualiza o nó original, removendo os valores que foram movidos para o auxiliar.
 * Cria um novo nó (split) representando a chave a ser promovida para o nível superior da árvore.
 * POr fim, escreve os nós no e auxiliar no arquivo de índices.
 * 
 * @param indice 
 * @param bHeader 
 * @param no - no que será splitado
 * @param newKey - nova chave a ser inserida 
 * @param PR 
 * @param P 
 * @return Node split - chave, RRN e ponteiro do nó a direita
 */
Node split(FILE* indice, btree_header *bHeader, Node* no, char* newKey, int PR, int P) {
    // Verifica se ta no nó raiz, se estiver, splita e cria outro nó

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
    auxiliar.RRNdoNO = bHeader->RRNproxNo;
    bHeader->RRNproxNo++;

    // Apaga valores do no que foram retirados
    for (int i = 0; i < 3; i++) {
        no->P[i] = -1;
        no->P[i + 1] = -1;
        no->C[i][0] = NULL_TERM;
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


    Node split = criaNode();
    strcpy(split.C[0], temp[2]);
    split.PR[0] = PRtemp[2];
    split.P[1] = auxiliar.RRNdoNO;
    split.P[0] = no->RRNdoNO; 
    // Escreve os nós no arquivo de indices, e retorna o cabecalho para a funcao 
    
    escreve_node(indice, no);
    escreve_node(indice, &auxiliar);
    // Retorna a chave, RRN e ponteiro do nó a direita
    return split;
}

/**
 * @brief Inserção de uma nova chave em um nó. Se necessário, realiza a divisão do nó e propaga a promoção de uma
 * chave para o nível superior, garantindo que a estrutura da árvore seja mantida.
 * Casos possíveis:
 * a) Se a árvore está vazia, cria um novo nó, insere a chave e o escreve no arquivo.
 * b) Se o nó não é uma folha, realiza uma chamada recursiva para encontrar a folha onde a chave deve ser inserida.
 * Caso a folha onde a inserção ocorreu cresceu, atualiza a chave e os ponteiros necessários.
 * Verifica se o nó está cheio após a inserção. Se não estiver, simplesmente insere a chave.
 * Se o nó estiver cheio, realiza a divisão do nó e propaga a promoção.
 * Se o nó dividido é a raiz, cria uma nova raiz e atualiza o cabeçalho da árvore.
 * Se não for a raiz, retorna o nó dividido com um marcador de promoção.
 * 
 * @param indice 
 * @param newKey 
 * @param PR 
 * @param no 
 * @param RRN_pai 
 * @param bHeader 
 * @return Node 
 */
Node InserirNo(FILE *indice, char* newKey, int PR, Node no, int RRN_pai, btree_header *bHeader){
    // 1. Procurar a folha onde a chave deve ser inserida
     
    int cresceu = 0;
    int P = -1;
    Node aux;
    if (RRN_pai == -1){
        RRN_pai = bHeader->noRaiz;
        read_node(indice, &no, RRN_pai);
    }

    if (bHeader->noRaiz == -1){
        // Caso da árvore Vazia
        no = criaNode();
        bHeader->noRaiz = 0;
        bHeader->RRNproxNo++;
        insereNoIndice(&no, newKey, PR, -1);
        escreve_node(indice, &no);
        return no; 
    }
    
    
    
    if (no.alturaNo != 1){
        Node prox = BuscaNoFolha(indice, no, newKey);

        aux = InserirNo(indice, newKey, PR, prox, prox.RRNdoNO, bHeader);
        strcpy(newKey, aux.C[0]);
        PR = aux.PR[0];
        P = aux.P[1];
        if (aux.P[0] == -2){
            cresceu = 1;
        } else {
            cresceu = 0;
        }
    } 
    // Verificando se a folha está cheia
    if (no.alturaNo == 1){
        cresceu = 1;
    }
    if (cresceu == 1){
        if (no.nroChavesNo != 3){
            insereNoIndice(&no, newKey, PR, P);
            escreve_node(indice, &no);
            return no; // Achou espaço e nao precisa mais subir nenhuma chave
        } else {
            if (no.RRNdoNO == bHeader->noRaiz){
                Node newRoot = split(indice, bHeader, &no, newKey, PR, P);
                newRoot.RRNdoNO = bHeader->RRNproxNo;
                bHeader->noRaiz = newRoot.RRNdoNO;
                bHeader->RRNproxNo++;
                newRoot.nroChavesNo = 1;
                newRoot.alturaNo = no.alturaNo + 1;
                escreve_node(indice, &newRoot);
                return no; // Inseriu uma nova raiz
            }
            else {
                Node splitado = split(indice, bHeader, &no, newKey, PR, P);
                cresceu = 1;
                splitado.P[0] = -2;
                return splitado;
            }
        }
    }

    return no;    
}