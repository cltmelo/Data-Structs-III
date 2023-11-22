#ifndef REGISTEROPERATIONS_H_INCLUDED
#define REGISTEROPERATIONS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoesFornecidas.h"

// Quantidade de bytes do registro de dados
#define TAM_REGISTRO 76
// Quantidade de bytes da parte fixa do registro
#define TAM_REGISTRO_FIXO 21
// Quantidade de bytes do cabeçalho
#define TAM_CABECALHO 13

// Caractere padrão para o preenchimento de bytes vazios no registro
#define LIXO '$'

// Caracteres padrão para remoção lógica
#define REMOVIDO '1'
#define NAO_REMOVIDO '0'

#define NULL_TERM '\0'


// Cabeçalho com 13 bytes
typedef struct {
    char status;  //Pode assumir os valores ‘0’, para indicar que o arquivo de dados está inconsistente, ou ‘1’, para indicar que o arquivo de dados está consistente. Ao se abrir um arquivo para escrita, seu status deve ser ‘0’ e, ao finalizar o uso desse arquivo, seu status deve ser ‘1’ – tamanho: string de 1 byte.
    int proxRRN; //Deve ser iniciado com o valor ‘0’ e deve ser alterado sempre que necessário – tamanho: inteiro de 4 bytes.
    int nroTecnologias; //Note que, se duas ou mais tecnologias têm o mesmo nome, elas são consideradas a mesma tecnologia. Deve ser iniciado com o valor ‘0’ e deve ser alterado sempre que necessário – tamanho: inteiro de 4 bytes.
    int nroParesTecnologias; //Deve ser iniciado com o valor ‘0’ e deve ser alterado sempre que necessário – tamanho: inteiro de 4 bytes.
} Cabecalho;

typedef struct {
    int tamanho;
    char* string;
} StringVariavel;

// Registro com 76 bytes
typedef struct {
    char removido; // '1' (REMOVIDO) para removido e '0' (NAO_REMOVIDO) para não removido

    // seguintes campos presentes no arquivo .csv fornecido
    int grupo;
    int popularidade;
    int peso;
    StringVariavel tecnologiaOrigem;
    StringVariavel tecnologiaDestino;

    
} Registro;

Registro* inicializarRegistro();

Cabecalho* inicializarCabecalho();

// Função para abrir um arquivo binário para leitura
FILE *abrirArquivoLeitura(const char *nomeArquivo);

// Função para abrir um arquivo binário para escrita
FILE *abrirArquivoEscrita(const char *nomeArquivo);

// Função para fechar um arquivo binário
void fecharArquivo(FILE *arquivo);

// Função para ler um registro do arquivo binário
int lerRegistro(FILE *arquivo, Registro *registro);

// Função para escrever um registro no arquivo binário
void escreverRegistro(FILE *arquivo, const Registro *registro);

// Função para preencher os campos vaios do registro atual comm 'LIXO'
void preenche_lixo(FILE* arquivo, Registro* registro);

// Função para escrever os dados do registro no cabeçalho
void escreverCabecalho(FILE* arquivo, Cabecalho *c);

// Função para ler cabecalho
int lerCabecalho(FILE* arquivo, Cabecalho* c);

// Função para atualizar as informações do cabeçalho
void atualizaCabecalho(Registro* registro, Cabecalho* cabecalho);

// Função para imprimir o registro de dados
void printRegister(Registro *registro);

#endif // REGISTEROPERATIONS_H_INCLUDED