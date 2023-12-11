#include <stdio.h>
#include <stdlib.h>
#include "graphs.h"
#include "funcoesAuxiliares.h"
//Definição do tipo Grafo
struct grafo{
    int nro_vertices;
    int grau_max;
    int** arestas;
    float** pesos;
    int* grau_entrada;
    int* grau_saida;
    int *grupo;
};

Grafo* cria_Grafo(int nro_vertices, int grau_max){
    Grafo *gr;
    gr = (Grafo*) malloc(sizeof(struct grafo));
    if(gr != NULL){
        int i;
        gr->nro_vertices = nro_vertices;
        gr->grau_max = grau_max;
        gr->grau_entrada = (int*) calloc(nro_vertices,sizeof(int));
        gr->grau_saida = (int*) calloc(nro_vertices,sizeof(int));
        gr->grupo = (int*) calloc (nro_vertices, sizeof(int));
        gr->arestas = (int**) malloc(nro_vertices * sizeof(int*));
        for(i=0; i<nro_vertices; i++)
            gr->arestas[i] = (int*) malloc(grau_max * sizeof(int));

        gr->pesos = (float**) malloc(nro_vertices * sizeof(float*));
        for(i=0; i<nro_vertices; i++)
            gr->pesos[i] = (float*) malloc(grau_max * sizeof(float));
        

    }
    return gr;
}

void libera_Grafo(Grafo* gr){
    if(gr != NULL){
        int i;
        for(i=0; i<gr->nro_vertices; i++)
            free(gr->arestas[i]);
        free(gr->arestas);
        for(i=0; i<gr->nro_vertices; i++)
            free(gr->pesos[i]);
        free(gr->pesos);
        free(gr->grau_saida);
        free(gr->grau_entrada);
        free(gr->grupo);
        free(gr);

    }
}

int insereAresta(Grafo* gr, int orig, int dest, float peso, int grupo){
    if(gr == NULL)
        return 0;
    if(orig < 0 || orig >= gr->nro_vertices)
        return 0;
    if(dest < 0 || dest >= gr->nro_vertices)
        return 0;

    gr->arestas[orig][gr->grau_saida[orig]] = dest;
    gr->pesos[orig][gr->grau_saida[orig]] = peso;
    gr->grau_saida[orig]++;
    gr->grau_entrada[dest]++;
    gr->grupo[orig] = grupo;
    return 1;
}

void imprime_Grafo(Grafo *gr, Lista *li){
    if(gr == NULL)
        return;
    int i, j;
    int* aux = indicesEmOrdemAlfabetica(li, tamanho_lista(li));

    for(i=0; i < gr->nro_vertices; i++){
        for (int k = 0; k < tamanho_lista(li); k++){
            for(j=0; j < gr->grau_saida[aux[i] + 1]; j++){
                if ((aux[k] + 1) == gr->arestas[aux[i] + 1][j]){
                    printTec(li, aux[i] + 1);
                    printf(", ");
                    printf("%d, ", gr->grupo[aux[i] + 1]);
                    printf("%d, %d, %d, ", gr->grau_entrada[aux[i] + 1], gr->grau_saida[aux[i] + 1], gr->grau_saida[aux[i] + 1] + gr->grau_entrada[aux[i] + 1]);
                    printTec(li, gr->arestas[aux[i] + 1][j]);
                    printf(", %0.f", gr->pesos[aux[i] + 1][j]);
                    printf("\n");
                }
                
            }
        }
        
    }
}


void imprime_GrafoT(Grafo *gr, Lista *li){
    if(gr == NULL)
        return;
    int i, j;
    int* aux = indicesEmOrdemAlfabetica(li, tamanho_lista(li));

    for(i=0; i < gr->nro_vertices; i++){
        for (int k = 0; k < tamanho_lista(li); k++){
            for(j=0; j < gr->grau_saida[aux[i] + 1]; j++){
                if ((aux[k] + 1) == gr->arestas[aux[i] + 1][j]){
                    printTec(li, aux[i] + 1);
                    printf(", ");
                    printf("%d, ", gr->grupo[aux[i] + 1]);
                    printf("%d, %d, %d, ", gr->grau_entrada[aux[i] + 1], gr->grau_saida[aux[i] + 1], gr->grau_saida[aux[i] + 1] + gr->grau_entrada[aux[i] + 1]);
                    printTec(li, gr->arestas[aux[i] + 1][j]);
                    printf(", %0.f", gr->pesos[aux[i] + 1][j]);
                    printf("\n");
                }
                
            }
        }
        
    }
}


void imprimirTecGerada(Grafo *gr, Lista *li, char* tec){
    if(gr == NULL)
        return;
    int i, j;
    int aux = busca_sequencial(li, tec);
    int *tecnologias;
    printf("%s: ", tec);
    tecnologias = (int*) malloc (sizeof(int) * gr->grau_saida[aux]);
    for(j=0; j < gr->grau_saida[aux]; j++){
        tecnologias[j] = gr->arestas[aux][j];
    }
    int contador = 0;
    int* tecs = indicesEmOrdemAlfabetica(li, tamanho_lista(li));
    for (i = 0; i < tamanho_lista(li); i++){
        for (j = 0; j < gr->grau_saida[aux]; j++){
            if (tecs[i] + 1 == tecnologias[j]){
                printTec(li, tecnologias[j]);
                contador++;
                if (contador < gr->grau_saida[aux]){
                    printf(", ");
                } else {
                    printf("\n\n");
                }
            }
        }
    }    
}

int procuraMenorDistancia(float *dist, int *visitado, int NV){
    int i, menor = -1, primeiro = 1;
    for(i=0; i < NV; i++){
        if(dist[i] >= 0 && visitado[i] == 0){
            if(primeiro){
                menor = i;
                primeiro = 0;
            }else{
                if(dist[menor] > dist[i])
                    menor = i;
            }
        }
    }
    return menor;
}

void menorCaminho_Grafo(Grafo *gr, int ini, int *ant, float *dist){
    int i, cont, NV, ind, *visitado, vert;
    cont = NV = gr->nro_vertices;
    visitado = (int*) malloc(NV * sizeof(int));
    for(i=0; i < NV; i++){
        ant[i] = -1;
        dist[i] = -1;
        visitado[i] = 0;
    }
    dist[ini] = 0;
    while(cont > 0){
        vert = procuraMenorDistancia(dist, visitado, NV);
        //printf("u = %d\n",u);
        if(vert == -1)
            break;

        visitado[vert] = 1;
        cont--;
        for(i=0; i<gr->grau_saida[vert]; i++){
            ind = gr->arestas[vert][i];
            if(dist[ind] < 0){
               dist[ind] = dist[vert] + 1;//ou peso da aresta
               ant[ind] = vert;
            }else{
                if(dist[ind] > dist[vert] + 1){
                    dist[ind] = dist[vert] + 1;//ou peso da aresta
                    ant[ind] = vert;
                }
            }
        }
    }

    free(visitado);
}


Grafo*  geraGrafo(FILE* bin, Lista* lista){
    Registro* registro = inicializarRegistro();
    Cabecalho* cabecalho = inicializarCabecalho();

    if(!lerCabecalho(bin, cabecalho)){
        printf("Registro Inexistente.\n");
        return;
    }
    Grafo* gr = cria_Grafo(cabecalho->nroTecnologias, 20);
    while (fread(&(registro->removido), sizeof(char), 1, bin) == 1) {
        fread(&(registro->grupo), sizeof(int), 1, bin);
        fread(&(registro->popularidade), sizeof(int), 1, bin);
        fread(&(registro->peso), sizeof(int), 1, bin);
        
        fread(&(registro->tecnologiaOrigem.tamanho), sizeof(int), 1, bin);
        registro->tecnologiaOrigem.string = (char *)malloc(registro->tecnologiaOrigem.tamanho + 1);
        fread(registro->tecnologiaOrigem.string, registro->tecnologiaOrigem.tamanho, 1, bin);
        registro->tecnologiaOrigem.string[registro->tecnologiaOrigem.tamanho] = NULL_TERM;


        fread(&(registro->tecnologiaDestino.tamanho), sizeof(int), 1, bin);
        registro->tecnologiaDestino.string = (char *)malloc(registro->tecnologiaDestino.tamanho + 1);
        fread(registro->tecnologiaDestino.string, registro->tecnologiaDestino.tamanho, 1, bin);
        registro->tecnologiaDestino.string[registro->tecnologiaDestino.tamanho] = NULL_TERM;

        int tamRegistro = TAM_REGISTRO_FIXO + registro->tecnologiaDestino.tamanho + registro->tecnologiaOrigem.tamanho;
        char resto[TAM_REGISTRO-tamRegistro];
        fread(resto, 1, TAM_REGISTRO-tamRegistro, bin);
        if (registro->removido == NAO_REMOVIDO && registro->tecnologiaDestino.tamanho != 0 && registro->tecnologiaOrigem.tamanho != 0){
            insere_lista_final(lista, registro->tecnologiaOrigem.string);
            insere_lista_final(lista, registro->tecnologiaDestino.string);

            insereAresta(gr, busca_sequencial(lista, registro->tecnologiaOrigem.string), busca_sequencial(lista, registro->tecnologiaDestino.string), registro->peso, registro->grupo);
        }
    }


    return gr;
}