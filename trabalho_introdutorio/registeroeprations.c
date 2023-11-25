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

void removeLixo(char *str, int tamanho) {
    // Encontrar a posição do primeiro caractere de lixo ('$')
    int pos = 0;
    while (pos < tamanho && str[pos] != '$') {
        pos++;
    }
    strncpy(str, str, pos);
    str[pos] = '\0';
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

/*=============================================================================================================================*/

int ler_registro(FILE* arquivo, Registro* reg){
    // leitura do registro
    if(fread(&(reg->removido), sizeof(char), 1, arquivo) == 0) {
        // retorno com fim do arquivo
        return 1;
    }
    // leitura dos dados numericos iniciais
    fread(&(reg->grupo), sizeof(int), 1, arquivo);
    fread(&(reg->popularidade), sizeof(int), 1, arquivo);
    fread(&(reg->peso), sizeof(int), 1, arquivo);

    // leitura do tamanho do registro de texto inicial e alocacao da string necessaria
    fread(&(reg->tecnologiaOrigem.tamanho), sizeof(int), 1, arquivo);
    reg->tecnologiaOrigem.string = malloc((reg->tecnologiaOrigem.tamanho + 1) * sizeof(char));

    // leitura da string e preenchimento do terminador nulo
    fread(reg->tecnologiaOrigem.string, sizeof(char), reg->tecnologiaOrigem.tamanho, arquivo);
    reg->tecnologiaOrigem.string[reg->tecnologiaOrigem.tamanho] = '\0';

    // leitura do tamanho do segundo registro de texto e alocacao da string necessaria
    fread(&(reg->tecnologiaDestino.tamanho), sizeof(int), 1, arquivo);
    reg->tecnologiaDestino.string = malloc((reg->tecnologiaDestino.tamanho + 1) * sizeof(char));

    // leitura da string e preenchimento do terminador nulo
    fread(reg->tecnologiaDestino.string, sizeof(char), reg->tecnologiaDestino.tamanho, arquivo);
    reg->tecnologiaDestino.string[reg->tecnologiaDestino.tamanho] = '\0';

    // calcula tamanho do lixo e o pula
    int aux = (TAM_REGISTRO - (TAM_REGISTRO_FIXO + reg->tecnologiaOrigem.tamanho + reg->tecnologiaDestino.tamanho));
    fseek(arquivo, aux, SEEK_CUR);

    // retorno com sucesso
    return 0;
}

// int ler_campo(FILE* arquivo, char** valCampo, char* nomeCampo);
int ler_header(FILE* arquivo, Cabecalho* cabecalho){
    // le os dados contidos no registro de cabecalho
    if(fread(&cabecalho->status, sizeof(char), 1, arquivo) == 0) {
        return 1;
    }
    fread(&cabecalho->proxRRN, sizeof(int), 1, arquivo);
    fread(&cabecalho->nroTecnologias, sizeof(int), 1, arquivo);
    fread(&cabecalho->nroParesTecnologias, sizeof(int), 1, arquivo);

    return 0;
}

int ler_campo(FILE* arquivo, char** valCampo, char* nomeCampo) {
    char removido = '1'; // variavel para armazenar status de remoção do registro, com o intuito de identificar fim do arquivo.
    int temp; // variavel para lidar com campos inteiros
    int tamanho; // variavel para salvar tamanho de campos de texto

    // leitura do registro
    if(fread(&(removido), sizeof(char), 1, arquivo) == 0) {
        // retorno com fim do arquivo
        return 1;
    }

    // Posiciona cabeça leitora no inicio do campo para leitura e extrai valor do campo de acordo com o nome do campo dado.
    if(strcmp(nomeCampo, "grupo") == 0){
        fread(&temp, sizeof(int), 1, arquivo);
        snprintf(*valCampo, sizeof(char) * 80, "%d", temp);
    }else if(strcmp(nomeCampo, "popularidade") == 0){
        fseek(arquivo, 4, SEEK_CUR);
        fread(&temp, sizeof(int), 1, arquivo);
        snprintf(*valCampo, sizeof(char) * 80, "%d", temp);
    }else if(strcmp(nomeCampo, "peso") == 0){
        fseek(arquivo, 8, SEEK_CUR);
        fread(&temp, sizeof(int), 1, arquivo);
        snprintf(*valCampo, sizeof(char) * 80, "%d", temp);
    }else if(strcmp(nomeCampo, "nomeTecnologiaOrigem") == 0){
        fseek(arquivo, 12, SEEK_CUR);
        fread(&tamanho, sizeof(int), 1, arquivo);
        fread(*valCampo, sizeof(char), tamanho, arquivo);
        (*valCampo)[tamanho] = '\0';
    }else if(strcmp(nomeCampo, "nomeTecnologiaDestino") == 0){
        fseek(arquivo, 12, SEEK_CUR);
        fread(&tamanho, sizeof(int), 1, arquivo);
        fseek(arquivo, tamanho, SEEK_CUR);
        fread(&tamanho, sizeof(int), 1, arquivo);
        fread(*valCampo, sizeof(char), tamanho, arquivo);
        (*valCampo)[tamanho] = '\0';
    }else{
        return -1;
    }

    // retorno com sucesso
    return 0;
}