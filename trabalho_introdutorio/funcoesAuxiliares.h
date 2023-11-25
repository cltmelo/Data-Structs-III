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
void imprimeLista(Lista* li);
char* concat_string(Registro *reg);


int byte_offset(int RRN); //retorna o byte offset de um registros

Registro* separaString(const char* Linha); // Função qeu converte uma linha de texto em um formato para Registro


#endif /* FUNCOESAUXILIARES_H */