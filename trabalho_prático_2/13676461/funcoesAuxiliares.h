/*
* Name: Lucas Corlete Alves de Melo - NUSP: 13676461; Jean Carlos Pereira Cassiano - NUSP: 138640008
* Course: SCC0607 - Estrutura de Dados III
* Professor: Cristina Dutra de Aguiar
* Project: Trabalho Introdutório, 1 e 2 de ED3
* Description: Este trabalho tem como objetivo armazenar dados em um arquivo binário bem como desenvolver funcionalidades para a 
* manipulação desses dados. Novas funcionalidades serão adicionadas conforme o avançar da disciplina.
*/

#ifndef FUNCOESAUXILIARES_H
#define FUNCOESAUXILIARES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "registeropertions.h"

/*
-----------------------------------------------------------------------------------------------
Header para funcionalidades e structs auxilares que utilizamos, tal como a lista ligada.
-----------------------------------------------------------------------------------------------
*/

typedef char data;

struct elemento{
    data* valor;
    struct elemento *prox;
};

typedef struct elemento Elem; 

typedef struct elemento* Lista;

Lista* cria_lista();
void libera_lista(Lista* li);
void insere_lista_final(Lista *li, data *x);
int tamanho_lista(Lista* li);
// void imprimeLista(Lista* li);
int busca_sequencial(Lista *li, data *chave);
void printTec(Lista *li, int indice);
int* indicesEmOrdemAlfabetica(Lista* li, int* tamanho);



int byte_offset(int RRN); //retorna o byte offset de um registros

Registro* separaString(const char* Linha); // Função qeu converte uma linha de texto em um formato para Registro




data* recupera_lista_pos(Lista* li, int pos);
Lista* inicializarListaTecnologias(FILE* arquivo, int n);


#endif /* FUNCOESAUXILIARES_H */