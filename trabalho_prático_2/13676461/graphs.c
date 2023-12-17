#include <stdio.h>
#include <stdlib.h>
#include "graphs.h"
#include "funcoesAuxiliares.h"

/**
 * @brief Cria um grafo com o número especificado de vértices e grau máximo.
 * 
 * Esta função aloca dinamicamente a estrutura de grafo e seus elementos internos com base no número de vértices e grau máximo especificados.
 * 
 * @param nro_vertices Número de vértices do grafo.
 * @param grau_max Grau máximo do grafo.
 * @return Grafo* gr, ponteiro para struct Grafo criado.
 */
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

/**
 * @brief Libera a memória alocada para a estrutura de grafo.
 * 
 * @param gr Ponteiro para o struct Grafo a ser liberado.
 */
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

/**
 * @brief Insere uma aresta no grafo, especificando os vértices de origem e destino, o peso da aresta e o grupo ao qual pertence.
 * 
 * @param gr
 * @param orig Vértice de origem.
 * @param dest Vértice de destino.
 * @param peso Peso da aresta.
 * @param grupo Grupo ao qual a aresta pertence.
 * @return 1 se a inserção for bem-sucedida, 0 caso contrário.
 */
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

/**
 * @brief Imprime o grafo, considerando a ordem alfabética das tecnologias.
 * 
 * As informações impressas incluem o vértice de origem, o grupo, os graus de entrada e saída, o vértice de destino, o peso da aresta.
 * 
 * @param gr
 * @param li Lista de tecnologias.
 */
void imprime_Grafo(Grafo *gr, Lista *li){
    if(gr == NULL)
        return;
    int i, j;
    int* aux = indicesEmOrdemAlfabetica(li, tamanho_lista(li));

    for(i=0; i < gr->nro_vertices - 1; i++){
        for (int k = 0; k < tamanho_lista(li); k++){
            for(j=0; j < gr->grau_saida[aux[i] + 1]; j++){
                if ((aux[k] + 1) == gr->arestas[aux[i] + 1][j]){
                    printTec(li, aux[i] + 1);
                    printf(" ");
                    printf("%d ", gr->grupo[aux[i] + 1]);
                    printf("%d %d %d ", gr->grau_entrada[aux[i] + 1], gr->grau_saida[aux[i] + 1], gr->grau_saida[aux[i] + 1] + gr->grau_entrada[aux[i] + 1]);
                    printTec(li, gr->arestas[aux[i] + 1][j]);
                    printf(" %0.f", gr->pesos[aux[i] + 1][j]);
                    printf("\n");
                }
                
            }
        }
        
    }
}

/**
 * @brief Gera o grafo transposto, com base no grafo de entrada.
 * 
 * @param gr
 * @return Grafo* ponteiro para o grafo transposto.
 */
Grafo*  geraTransposto(Grafo* gr){
    Grafo* trans = cria_Grafo(gr->nro_vertices, 20);
    int i, j;
    for(i=0; i < gr->nro_vertices; i++){
        for(j=0; j < gr->grau_saida[i]; j++){
            insereAresta(trans, gr->arestas[i][j], i, gr->pesos[i][j], gr->grupo[i]);
        }
    }
    return trans;

}

/**
 * @brief Imprime o grafo transposto, considerando a ordem alfabética das tecnologias.
 
 * @param gr
 * @param li Lista de tecnologias.
 */
void imprime_GrafoT(Grafo *gr, Lista *li){
    if(gr == NULL)
        return;
    int i, j;
    int* aux = indicesEmOrdemAlfabetica(li, tamanho_lista(li));
    Grafo *trans;
    trans = geraTransposto(gr);
    for(i=0; i < trans->nro_vertices - 1; i++){
        for (int k = 0; k < tamanho_lista(li); k++){
            for(j=0; j < trans->grau_saida[aux[i] + 1]; j++){
                if ((aux[k] + 1) == trans->arestas[aux[i] + 1][j]){
                    printTec(li, aux[i] + 1);
                    printf(" %d ", gr->grupo[aux[i] + 1]);
                    printf("%d %d %d ", trans->grau_entrada[aux[i] + 1], trans->grau_saida[aux[i] + 1], trans->grau_saida[aux[i] + 1] + trans->grau_entrada[aux[i] + 1]);
                    printTec(li, trans->arestas[aux[i] + 1][j]);
                    printf(" %0.f", trans->pesos[aux[i] + 1][j]);
                    printf("\n");
                }
                
            }
        }
        
    }
}



/**
 * @brief Imprime as tecnologias alcançáveis a partir de uma tecnologia específica no grafo transposto.
 * 
 * @param gr Ponteiro para o grafo original.
 * @param li Lista de tecnologias.
 * @param tec Nome da tecnologia de origem.
 */
void imprimirTecGerada(Grafo *gr, Lista *li, char* tec){
    if(gr == NULL)
        return;
    int i, j;
    int aux = busca_sequencial(li, tec);
    int *tecnologias;
    printf("%s: ", tec);
    Grafo *trans = geraTransposto(gr);
    tecnologias = (int*) malloc (sizeof(int) * (trans->grau_saida[aux]));
    for(j=0; j < trans->grau_saida[aux]; j++){
        tecnologias[j] = trans->arestas[aux][j];
    }
    i = 0;

    int contador = 0;
    int* tecs = indicesEmOrdemAlfabetica(li, tamanho_lista(li));
    for (i = 0; i < tamanho_lista(li); i++){
        for (j = 0; j < trans->grau_saida[aux]; j++){
            if (tecs[i] + 1 == tecnologias[j]){
                printTec(li, tecnologias[j]);
                contador++;
                if (contador < trans->grau_saida[aux]){
                    printf(", ");
                } else {
                    printf("\n\n");
                }
            }
        }
    }    
}

/**
 * @brief Procura o vértice não visitado com a menor distância no vetor dist.
 * 
 * @param dist Vetor de distâncias.
 * @param visitado Vetor de vértices visitados.
 * @param NV Número de vértices.
 * @return int menor, indice do vértice não visitado com a menor distância. Retorna -1 se não encontrar.
 */
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

/**
 * @brief Encontra o menor caminho em um grafo a partir de um vértice inicial.
 * 
 * @param gr Grafo.
 * @param ini Vértice inicial.
 * @param ant Vetor de predecessores no menor caminho.
 * @param dist Vetor de distâncias acumuladas.
 */
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


/**
 * @brief Gera um grafo a partir do binário e uma lista de tecnologias.
 * 
 * @param bin binário.
 * @param lista Lista de tecnologias.
 * @return Grafo* gr, ponteiro para o grafo gerado.
 */
Grafo*  geraGrafo(FILE* bin, Lista* lista){
    Registro* registro = inicializarRegistro();
    Cabecalho* cabecalho = inicializarCabecalho();

    if(!lerCabecalho(bin, cabecalho)){
        printf("Registro Inexistente.\n");
        return NULL;
    }
    Grafo* gr = cria_Grafo(cabecalho->nroTecnologias + 1, 20);
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


//=====================================================================================================================
// TENATIVA DA FUNCIONALIDADE 11


int dfs(Grafo* gr, int vert, int* visitado, int* pilha, int* tempo, int* componentes) {
    visitado[vert] = 1;

    for (int i = 0; i < gr->grau_saida[vert]; i++) {
        int ind = gr->arestas[vert][i];
        if (!visitado[ind]) {
            *tempo += 1;
            dfs(gr, ind, visitado, pilha, tempo, componentes);
        }
    }

    pilha[*tempo] = vert;
    *tempo += 1;
    return 1;
}

Grafo* transporGrafo(Grafo* gr) {
    Grafo* gr_transposto = cria_Grafo(gr->nro_vertices, gr->grau_max);

    if (gr_transposto != NULL) {
        for (int i = 0; i < gr->nro_vertices; i++) {
            for (int j = 0; j < gr->grau_saida[i]; j++) {
                int dest = gr->arestas[i][j];
                float peso = gr->pesos[i][j];
                insereAresta(gr_transposto, dest, i, peso, gr->grupo[i]);
            }
        }
    }

    return gr_transposto;
}

/* SO PARA TENTAR ENTENDER O QUE ESTÁ ACONTECENDO
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
*/

void dfsComponentes(Grafo* gr, int vert, int* visitado, int componente) {
    visitado[vert] = 1;
    gr->grupo[vert] = componente;

    for (int i = 0; i < gr->grau_saida[vert]; i++) {
        int ind = gr->arestas[vert][i];
        if (!visitado[ind]) {
            dfsComponentes(gr, ind, visitado, componente);
        }
    }
}

void componentesFortementeConexos(Grafo* gr) {
    int* visitado = (int*)malloc(gr->nro_vertices * sizeof(int));
    int* pilha = (int*)malloc(gr->nro_vertices * sizeof(int));
    int tempo = 0;
    int componente = 0;

    // Inicializa o array de visitados
    for (int i = 0; i < gr->nro_vertices; i++) {
        visitado[i] = 0;
    }

    // Preenche a pilha com os vértices em ordem de término de DFS
    for (int i = 0; i < gr->nro_vertices; i++) {
        if (!visitado[i]) {
            dfs(gr, i, visitado, pilha, &tempo, &componente);
        }
    }

    // Transpõe o grafo
    Grafo* gr_transposto = transporGrafo(gr);

    // Reinicializa o array de visitados
    for (int i = 0; i < gr->nro_vertices; i++) {
        visitado[i] = 0;
    }

    // Processa os vértices em ordem da pilha para encontrar componentes fortemente conexos
    for (int i = gr->nro_vertices - 1; i >= 0; i--) {
        int vert = pilha[i];
        if (!visitado[vert]) {
            componente++;
            dfsComponentes(gr_transposto, vert, visitado, componente);
        }
    }

    // Conta o número de componentes fortemente conexos
    int num_componentes = componente;

    // Imprime o resultado
    if (num_componentes == 1) {
        printf("Sim, o grafo é fortemente conexo e possui 1 componente.\n");
    } else {
        printf("Nao, o grafo nao e fortemente conexo e possui %d componentes.\n", num_componentes);
    }

    // Libera a memória
    free(visitado);
    free(pilha);
    libera_Grafo(gr_transposto);
}






// int dfsOrdemTermino(Grafo* gr, int vert, int* visitado, int* pilha, int* tempo) {
//     visitado[vert] = 1;

//     for (int i = 0; i < gr->grau_saida[vert]; i++) {
//         int ind = gr->arestas[vert][i];
//         if (!visitado[ind]) {
//             *tempo += 1;
//             dfsOrdemTermino(gr, ind, visitado, pilha, tempo);
//         }
//     }

//     pilha[*tempo] = vert;
//     *tempo += 1;
//     return 1;
// }


//=====================================================================================================================
// TENATIVA DA FUNCIONALIDADE 12

// Função para obter o peso da aresta entre dois vértices
float obterPesoAresta(Grafo* gr, int origem, int destino) {
    return gr->pesos[origem][destino];
}

// Função para obter o nome da tecnologia associada a um vértice
char* obterTecnologia(Grafo* gr, int vertice) {
    return gr->tecnologias[vertice];
}

// Função para inicializar o Grafo com nomes das tecnologias
Grafo* criaGrafoComTecnologias(int nro_vertices, int grau_max, Lista* lista_tecnologias) {
    Grafo *gr;
    gr = (Grafo*) malloc(sizeof(struct grafo));
    if(gr != NULL){
        int i;
        gr->nro_vertices = nro_vertices;
        gr->grau_max = grau_max;
        gr->grau_entrada = (int*) calloc(nro_vertices, sizeof(int));
        gr->grau_saida = (int*) calloc(nro_vertices, sizeof(int));
        gr->grupo = (int*) calloc(nro_vertices, sizeof(int));
        gr->arestas = (int**) malloc(nro_vertices * sizeof(int*));
        for(i = 0; i < nro_vertices; i++)
            gr->arestas[i] = (int*) malloc(grau_max * sizeof(int));

        gr->pesos = (float**) malloc(nro_vertices * sizeof(float*));
        for(i = 0; i < nro_vertices; i++)
            gr->pesos[i] = (float*) malloc(grau_max * sizeof(float));

        gr->tecnologias = (char**) malloc(nro_vertices * sizeof(char*));
        for(i = 0; i < nro_vertices; i++) {
            gr->tecnologias[i] = (char*) malloc(MAX_NOME_TECNOLOGIA * sizeof(char));
            strcpy(gr->tecnologias[i], recupera_lista_pos(lista_tecnologias, i));
            // strcpy(gr->tecnologias[i], recupera_lista_pos(lista_tecnologias, i + 1));  // Assumindo que a lista de tecnologias começa na posição 1
        }
    }
    return gr;
}


void dijkstra(Grafo* gr, int origem, int destino) {
    printf("\nOrigem: %d, Destino: %d\n", origem, destino); //DEPURACAO
    
    int numVertices = gr->nro_vertices;

    int* visitado = (int*)malloc(numVertices * sizeof(int));
    int* antecessor = (int*)malloc(numVertices * sizeof(int));
    float* distancia = (float*)malloc(numVertices * sizeof(float));

    // Inicialização
    for (int i = 0; i < numVertices; i++) {
        visitado[i] = 0;
        antecessor[i] = -1;
        distancia[i] = INFINITO;
    }

    // A distância da origem para ela mesma é sempre 0
    distancia[origem] = 0;

    for (int count = 0; count < numVertices - 1; count++) {
        // Escolhe o vértice de menor distância ainda não visitado
        int u = -1;
        for (int v = 0; v < numVertices; v++) {
            if (!visitado[v] && (u == -1 || distancia[v] < distancia[u])) {
                u = v;
            }
        }

        // Marca o vértice escolhido como visitado
        visitado[u] = 1;

        // Atualiza as distâncias dos vértices adjacentes ao vértice escolhido
        for (int v = 0; v < numVertices; v++) {
            float peso = obterPesoAresta(gr, u, v);  // Substitua pela função apropriada
            if (!visitado[v] && peso > 0 && distancia[u] != INFINITO && (distancia[u] + peso) < distancia[v]) {
                distancia[v] = distancia[u] + peso;
                antecessor[v] = u;
            }
        }
    }

    // Imprime o caminho mais curto e o peso
    if (distancia[destino] != INFINITO) {
        printf("\"%s\" \"%s\": %.2f\n", obterTecnologia(gr, origem), obterTecnologia(gr, destino), distancia[destino]);
    } else {
        printf("\"%s\" \"%s\": CAMINHO INEXISTENTE\n", obterTecnologia(gr, origem), obterTecnologia(gr, destino));
    }

    // Libera a memória alocada
    free(visitado);
    free(antecessor);
    free(distancia);
}