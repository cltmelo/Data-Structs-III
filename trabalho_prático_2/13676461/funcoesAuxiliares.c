/*
* Name: Lucas Corlete Alves de Melo - NUSP: 13676461; Jean Carlos Pereira Cassiano - NUSP: 138640008
* Course: SCC0607 - Estrutura de Dados III
* Professor: Cristina Dutra de Aguiar
* Project: Trabalho Introdutório, 1 e 2 de ED3
* Description: Este trabalho tem como objetivo armazenar dados em um arquivo binário bem como desenvolver funcionalidades para a 
* manipulação desses dados. Novas funcionalidades serão adicionadas conforme o avançar da disciplina.
*/

#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include "funcoesAuxiliares.h"

/*
-------------------------------------------------------------------------------------
*/

/**
 * @brief Cria TAD Lista
 * 
 * @return Lista li
 */
Lista* cria_lista(){
    Lista* li = (Lista*) malloc(sizeof(Lista));
    if (li != NULL){
        *li = NULL;
    }
    return li;
}

/**
 * @brief Libera memória alocada para criação de uma Lista
 * 
 * @param li 
 */
void libera_lista(Lista* li){
    if(li != NULL){
        Elem* no;
        while ((*li) != NULL){
            no = *li;
            *li = (*li) -> prox;
            free(no);
        }
        free(li);
    }
}

/**
 * @brief Inserção de elemento (data) no final da Lista li
 * 
 * @param li 
 * @param x (data) a ser inserido
 */
void insere_lista_final(Lista *li, data *x) {
    if (li == NULL) {
        // Retorna algo apropriado em caso de erro
        return;
    }

    Elem* no = (Elem*) malloc(sizeof(Elem));
    if (no == NULL) {
        // Retorna algo apropriado em caso de erro
        return;
    }

    // Aloca espaço para o valor e copia o conteúdo de x
    no->valor = (data*)malloc((strlen(x) + 1) * sizeof(data));
    strcpy(no->valor, x);
    
    no->prox = NULL;

    if ((*li) == NULL) {
        *li = no;
    } else {
        Elem *aux = *li;

        // Verifica se o valor já existe na lista
        while (aux->prox != NULL) {
            if (strcmp(aux->valor, x) == 0) {
                free(no->valor); // Liberar a memória alocada para o valor
                free(no); // Liberar a memória alocada para o nó
                return; // Valor já existe, não insere
            }
            aux = aux->prox;
        }

        // Verifica o último elemento da lista
        if (strcmp(aux->valor, x) == 0) {
            free(no->valor); // Liberar a memória alocada para o valor
            free(no); // Liberar a memória alocada para o nó
            return; // Valor já existe, não insere
        }

        // Valor não encontrado, insere no final da lista
        aux->prox = no;
    }
}

/**
 * @brief Função que retorna o tamanho atual da nossa lista
 * 
 * @param li 
 * @return count tamanho da lista 
 */
int tamanho_lista(Lista* li){
    if (li == NULL ){
        return 0;
    }
    int count = 0;
    Elem *no = *li;
    while(no != NULL){
        count++;
        no = no->prox;
    }
    return count;
}


void imprimeLista(Lista* li){
    if (li == NULL ){
        return;
    }
    Elem *no = *li;
    while(1){
        printf("\"%s\"\n", no->valor);
        if (no->prox == NULL){
            break;
        }
        no = no->prox;
    }
}

/*
-------------------------------------------------------------------------------------
*/



/**
 * @brief Calcula o byte offset, dado o RRN
 * 
 * @param RRN 
 * @return int 
 */
int byte_offset(int RRN){
    return (RRN * TAM_REGISTRO + TAM_CABECALHO);
}

/**
 * @brief Função para transformar linha do arquivo de dados para a variável do tipo registro.
 * Analisa uma string representando um registro e o cria a partir desses dados. A string de entrada é dividida em campos, separados por vírgulas.
 * A função aloca dinamicamente memória para armazenar as strings e retorna um ponteiro para a estrutura de registro resultante.
 * 
 * @param Linha 
 * @return Registro* - ponteiro para a estrutura de registro resultante.
 */
Registro* separaString(const char* Linha){
    Registro* registro = inicializarRegistro();
    int i = 0;
    char* aux = (char *)malloc(strlen(Linha) * sizeof(char));
    int j = 0;
    int tam = strlen(Linha);
    int novotam = tam;
    int primeiro = 0;
    while (i < tam) {
        if (Linha[i] == ',' && (i == 0 ||(i > 0 && Linha[i + 1] == ',') || i == (tam-2) )) {
            if (i == tam - 2){
                novotam += 3;
                aux = (char *)realloc(aux, novotam * sizeof(char));
                aux[j] = ',';
                aux[j+1] = '-';
                aux[j+2] = '1';
                
            } else if (i == 0 && primeiro == 0){
                novotam += 5;
                aux = (char *)realloc(aux, novotam * sizeof(char));
                aux[j] = 'N';
                aux[j+1]  = 'U';
                aux[j+2] = 'L';
                aux[j+3] = 'L';
                aux[j+4] = ',';
                primeiro = 1;
                i--;
            } else {
                novotam += 5;
                aux = (char *)realloc(aux, novotam * sizeof(char));
                aux[j] = ',';
                aux[j+1]  = 'N';
                aux[j+2] = 'U';
                aux[j+3] = 'L';
                aux[j+4] = 'L';
            }
            j += 5;   
        } else {
            aux[j] = Linha[i];
            j++;
        }
        i++;
    }
    aux = (char *)realloc(aux, (novotam + 1) * sizeof(char));
    aux[j] = NULL_TERM;

    // Inicializa a string variável com NULL (vazia)
    char *token = strtok((char *)aux, ",");
    if (token != NULL && strcmp(token, "NULL") != 0) {
        char *copy = malloc(sizeof(char)*(strlen(token)+1));
        strcpy(copy, token);
        registro->tecnologiaOrigem.tamanho = strlen(token);
        registro->tecnologiaOrigem.string = copy;
        token = strtok(NULL, ",");
        
    } else {
        registro->tecnologiaOrigem.tamanho = 0;
        token = strtok(NULL, ",");
    } 
    
    if (token != NULL && strcmp(token, "NULL") != 0) {
        registro->grupo = atoi(token);
        token = strtok(NULL, ",");
    } else {
        registro->grupo = -1;
        token = strtok(NULL, ",");
    }

    if (token != NULL && strcmp(token, "NULL") != 0) {
        registro->popularidade = atoi(token);
        token = strtok(NULL, ",");
    } else {
        registro->popularidade = -1;
        token = strtok(NULL, ",");
    }

    if (token != NULL && strcmp(token, "NULL") != 0) {
        char* copia = malloc(sizeof(char)*(strlen(token)+1));
        strcpy(copia, token);
        registro->tecnologiaDestino.tamanho = strlen(token);
        registro->tecnologiaDestino.string = copia;
        token = strtok(NULL, ",");
    } else {
        registro->tecnologiaDestino.tamanho = 0;
        token = strtok(NULL, ",");
    }
    
    if (token != NULL || strcmp(token, "NULL") != 0) {
        registro->peso = atoi(token);
    } else {
        registro->peso = -1;
    }
    if (registro->peso == 0){
        registro->peso = -1;
    } 

    
    registro->removido = NAO_REMOVIDO;
    free(aux);
    return registro;
}

/*
-------------------------------------------------------------------------------------
*/

int compararStrings(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}
int* indicesEmOrdemAlfabetica(Lista* li, int tamanho) {
    // Conta o número de elementos na lista
    int numElementos = 0;
    Elem* aux = *li;
    while (aux != NULL) {
        numElementos++;
        aux = aux->prox;
    }

    // Aloca um array de ponteiros para strings
    char** arrayStrings = (char**)malloc(numElementos * sizeof(char*));
    char** arrayStrings2 = (char**)malloc(numElementos * sizeof(char*));
    if (arrayStrings == NULL) {
        // Tratar erro de alocação
        return NULL;
    }

    // Copia as strings da lista para o array
    aux = *li;
    for (int i = 0; i < numElementos; i++) {
        arrayStrings[i] = aux->valor;
        arrayStrings2[i] = aux->valor;
        aux = aux->prox;
    }

    // Usa a função qsort para ordenar o array de strings
    qsort(arrayStrings2, numElementos, sizeof(char*), compararStrings);

    // Aloca um array de inteiros para armazenar os índices ordenados
    int* arrayIndices = (int*)malloc(numElementos * sizeof(int));
    if (arrayIndices == NULL) {
        // Tratar erro de alocação
        free(arrayStrings);
        return NULL;
    }

    // Preenche o array de índices com as posições originais das strings
    aux = *li;

    
    for (int i = 0; i < numElementos; i++) {
        // Encontra o índice da string no array original
        for (int j = 0; j < numElementos; j++){
            if (strcmp(arrayStrings2[i], arrayStrings[j]) == 0){
                arrayIndices[i] = j;

            }
        }
    }
    // Libera a memória alocada para o array de strings
    free(arrayStrings);
    // Retorna o array de índices
    return arrayIndices;
}

int busca_sequencial(Lista *li, data *chave) {
    if (li == NULL || chave == NULL) {
        // Retorna um valor indicando que a chave não foi encontrada
        return -1;
    }
    int count = 0;
    Elem *no = *li;
    while(no != NULL){
        count++;
        if (strcmp(no->valor, chave) == 0 ){
            return count;
        }
        no = no->prox;
    }
    return count;

    // Retorna um valor indicando que a chave não foi encontrada
    return -1;
}

void printTec(Lista *li, int indice) {
    if (li == NULL || indice < 0) {
        // Retorna NULL indicando que a lista é inválida ou o índice é inválido
        return;
    }

    Elem* aux = *li;
    int posicao = 1;

    // Percorre a lista sequencialmente
    while (aux != NULL) {
        // Verifica se a posição é a desejada
        if (posicao == indice) {
            // Retorna a chave correspondente à posição
            printf("%s", aux->valor);
            return;
        }

        // Avança para o próximo elemento na lista
        aux = aux->prox;
        posicao++;
    }

}