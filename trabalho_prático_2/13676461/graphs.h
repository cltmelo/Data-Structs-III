#include "funcoesAuxiliares.h"
#ifndef GRAPHS_H
#define GRAPHS_H

typedef struct grafo Grafo;

Grafo* cria_Grafo(int nro_vertices, int grau_max);
void libera_Grafo(Grafo* gr);
int insereAresta(Grafo* gr, int orig, int dest, float peso, int grupo);
void menorCaminho_Grafo(Grafo *gr, int ini, int *antecessor, float *distancia);

void imprime_Grafo(Grafo *gr, Lista *li);
void imprime_GrafoT(Grafo *gr, Lista *li);
Grafo*  geraGrafo(FILE* bin, Lista* lista);
void imprimirTecGerada(Grafo *gr, Lista *li, char* tec);



#endif