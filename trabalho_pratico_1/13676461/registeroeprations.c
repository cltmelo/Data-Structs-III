#include <stdio.h>
#include "registeropertions.h"

FILE *abrirArquivoLeitura(const char *nomeArquivo){
    FILE* arquivo;

    arquivo = fopen(nomeArquivo, "rb");

    if(arquivo == NULL){
        printf("Falha no processamento do arquivo.\n");
        return 0;
    }
    
    return arquivo; // Ponteiro no começo do arquivo
}

FILE *abrirArquivoEscrita(const char *nomeArquivo){
    FILE* arquivo;

    arquivo = fopen(nomeArquivo, "wb+");

    if(arquivo == NULL){
        printf("Falha no processamento do arquivo.\n");
        return 0;
    }
    
    return arquivo; // Ponteiro no começo do arquivo
}

void removeLixo(char *str, int tamanho) {
    // Encontrar a posição do primeiro caractere de lixo ('$')
    int pos = 0;
    while (pos < tamanho && str[pos] != '$') {
        pos++;
    }
    strncpy(str, str, pos);
    str[pos] = '\0';
}

void fecharArquivo(FILE *arquivo){
    if (arquivo != NULL) {
        fclose(arquivo);
    }
}


Registro* inicializarRegistro(){
    Registro *r = (Registro*)malloc(sizeof(Registro));
    if (r == NULL) {
        perror("Erro ao alocar memória para o registro.\n");
        exit(EXIT_FAILURE);
    }

    r->removido = NAO_REMOVIDO;
    r->grupo = -1;
    r->popularidade = -1;
    r->peso = -1;

    r->tecnologiaDestino.tamanho = 0;
    r->tecnologiaOrigem.tamanho = 0;

    r->tecnologiaDestino.string = NULL;
    r->tecnologiaOrigem.string = NULL;

    return r;
}

Cabecalho* inicializarCabecalho(){
    Cabecalho* cabecalho = malloc(sizeof(Cabecalho));
    cabecalho->status = '0'; 
    cabecalho->proxRRN = 0;   
    cabecalho->nroTecnologias = 0;  
    cabecalho->nroParesTecnologias = 0;
    
    return cabecalho;
}

int lerRegistro(FILE *arquivo, Registro *registro){

    //leitura do registro
    if((&(registro->removido), sizeof(char), 1, arquivo) == NAO_REMOVIDO)
        return 0; //ok na leitura deste campo

    fread(&(registro->grupo), sizeof(char), 1, arquivo);
    fread(&(registro->peso), sizeof(char), 1, arquivo);
    fread(&(registro->popularidade), sizeof(char), 1, arquivo);


    fread(&(registro->tecnologiaDestino.tamanho), sizeof(int), 1, arquivo);
    registro->tecnologiaDestino.string = malloc((registro->tecnologiaDestino.tamanho + 1)*sizeof(char)); //alocação para o nome, pela leitura do tamanho do registro

    fread(&(registro->tecnologiaOrigem.tamanho), sizeof(int), 1, arquivo);
    registro->tecnologiaOrigem.string = malloc((registro->tecnologiaOrigem.tamanho + 1)*sizeof(char)); 


    //leitura do nome e preenchimento com '\0' no final
    fread(registro->tecnologiaDestino.string, sizeof(char), registro->tecnologiaDestino.tamanho, arquivo);
    fread(registro->tecnologiaOrigem.string, sizeof(char), registro->tecnologiaOrigem.tamanho, arquivo);
    registro->tecnologiaDestino.string[registro->tecnologiaDestino.tamanho] = NULL_TERM;
    registro->tecnologiaOrigem.string[registro->tecnologiaOrigem.tamanho] = NULL_TERM;
    

    //Ignorando a leitura de lixo
    int aux = TAM_REGISTRO - (TAM_REGISTRO_FIXO + registro->tecnologiaOrigem.tamanho + registro->tecnologiaDestino.tamanho);
    fseek(arquivo, aux, SEEK_CUR);

    return 1; //leitura bem sucedida
}


void preenche_lixo(FILE* arquivo, Registro* registro){
    //variavel auxliar para definir o tamanho do lixo no registro atual
    int aux = (TAM_REGISTRO - (TAM_REGISTRO_FIXO + registro->tecnologiaOrigem.tamanho + registro->tecnologiaDestino.tamanho));
    
    char trash[aux];

    for(int i=0; i<aux; i++){
        trash[i] = LIXO;
    }

    fwrite(trash, sizeof(char), aux, arquivo);
}

void escreverRegistro(FILE *arquivo, const Registro *registro){
    fwrite(&(registro->removido), sizeof(char), 1, arquivo);
    fwrite(&(registro->grupo), sizeof(int), 1, arquivo);
    fwrite(&(registro->popularidade), sizeof(int), 1, arquivo);
    
    fwrite(&(registro->peso), sizeof(int), 1, arquivo);
    fwrite(&(registro->tecnologiaOrigem.tamanho), sizeof(int), 1, arquivo);
    fwrite(registro->tecnologiaOrigem.string, sizeof(char), registro->tecnologiaOrigem.tamanho, arquivo);
    fwrite(&(registro->tecnologiaDestino.tamanho), sizeof(int), 1, arquivo);
    fwrite(registro->tecnologiaDestino.string, sizeof(char), registro->tecnologiaDestino.tamanho, arquivo);
    //Após o preenchimento dos campos, completar  registro com lixo
    preenche_lixo(arquivo, registro);
}


int lerCabecalho(FILE* arquivo, Cabecalho* c){

    if(fread(&c->status, sizeof(char), 1, arquivo) == 0)
        return 0; //fim do arquivo

    fread(&c->proxRRN, sizeof(int), 1, arquivo);
    fread(&c->nroTecnologias, sizeof(int), 1, arquivo);
    fread(&c->nroParesTecnologias, sizeof(int), 1, arquivo);

    return 1; //leitura bem sucedida
}

void escreverCabecalho(FILE* arquivo, Cabecalho* c){
    fwrite(&(c->status), sizeof(char), 1, arquivo);
    fwrite(&(c->proxRRN), sizeof(int), 1, arquivo);
    fwrite(&(c->nroTecnologias), sizeof(int), 1, arquivo);
    fwrite(&(c->nroParesTecnologias), sizeof(int), 1, arquivo);
}

void atualizaCabecalho(Registro* registro, Cabecalho* cabecalho){
    if(registro->tecnologiaDestino.tamanho != 0 && registro->tecnologiaOrigem.tamanho != 0)
        cabecalho->nroParesTecnologias++;
    cabecalho->proxRRN++;
}

void printRegister(Registro *registro){

    char* end = ", ";
    char* skip_line = "\n";
    
    // Imprime o primeiro campo, caso exista. Caso não exista, printa NULO.
    if(registro->tecnologiaOrigem.tamanho == 0) {
        printf("NULO");
    }
    else {
        printf("%s", registro->tecnologiaOrigem.string);
    }
    printf("%s", end);

    // Imprime grupo e popularidade. Caso negativo (-1), imprime NULO
    if (registro->grupo == -1)
    {
        printf("NULO");
    }else{
        printf("%d", registro->grupo);
    }
    printf("%s", end);
    
    if (registro->popularidade == -1)
    {
        printf("NULO");
    }else{
        printf("%d", registro->popularidade);
    }
    printf("%s", end);

    // Imprime o outro campo de tecnologia, caso exista. Caso não exista, printa NULO.
    if(registro->tecnologiaDestino.tamanho == 0) {
        printf("NULO");
    }
    else {
        printf("%s", registro->tecnologiaDestino.string);
    }
    printf("%s", end);

    // Imprime o ulimo campo.
    if (registro->peso == -1)
    {
        printf("NULO");
    }else{
        printf("%d", registro->peso);
    }
    printf("%s", skip_line);
}

