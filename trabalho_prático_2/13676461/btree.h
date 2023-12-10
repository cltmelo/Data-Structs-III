#ifndef BTREE_H
#define BTREE_H


/* O tamanho do registro de cabeçalho deve ser de 205 bytes */
typedef struct {
    char status;
    int noRaiz;
    int RRNproxNo;
} btree_header;

/* O tamanho de cada registro de dados é de 205 bytes */
typedef struct {
    int nroChavesNo; // 4 bytes
    int alturaNo; // 4 bytes
    int RRNdoNO; // 4 bytes
    int P[4]; // 16 bytes
    int PR[3]; // 12 bytes
    char C[3][55]; // 165 bytes
} Node; 


/* Funcões da Árvore B */
btree_header criarArvoreB();
void escreve_btree_header(FILE *indice, btree_header *bHeader);
int byteoffset_bHeader(int rrn);
void escreve_node(FILE *indice, Node *no);
int Busca(FILE* indice, char *chave, int RRN);
void inserirChave(int chave);
void removerChave(int chave);
void read_node(FILE* indice, Node *no, int RRN);
void printNode(Node no);
btree_header LerHeader(FILE* indice);
Node InserirNo(FILE *indice, char* newKey, int PR, Node no, int RRN_pai, btree_header *bHeader);
Node criaNode();
#endif