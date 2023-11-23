#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include "funcoesAuxiliares.h"

/*
-------------------------------------------------------------------------------------
*/

Lista* cria_lista(){
    Lista* li = (Lista*) malloc(sizeof(Lista));
    if (li != NULL){
        *li = NULL;
    }
    return li;
}

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

void insere_lista_final(Lista *li, data *x){
    if (li == NULL){
        return;
    }
    Elem* no = (Elem*) malloc(sizeof(Elem));
    if (no == NULL){
        return;
    }

    // no->valor = x; //alterar aqui pra dar bom
    no->valor = malloc( (strlen(x) + 1)*sizeof(data) );
    no->prox = NULL;
    if((*li) == NULL){
        *li = no;
    }
    else {
        Elem *aux = *li;
        while (aux->prox != NULL){
            if (strcmp(aux->valor, x) == 0){
                return;
            }
            aux = aux->prox;

        }
        if (strcmp(aux->valor, x) != 0){
            no->valor = x;
            aux->prox = no;
        }
    }
}

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
        return 0;
    }
    Elem *no = *li;
    while(no != NULL){
        printf("\"%s\"\n", no->valor);
        no = no->prox;
    }
}

/*
-------------------------------------------------------------------------------------
*/



int byte_offset(int RRN){
    return (RRN * TAM_REGISTRO + TAM_CABECALHO);
}

// Função para transformar linha do arquivo de dados para a variável do tipo registro
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
    aux[j] = '\0';

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

    
    registro->removido = '0';
    free(aux);
    return registro;
}

/*
-------------------------------------------------------------------------------------
*/