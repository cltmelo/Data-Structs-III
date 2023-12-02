/*
* Name: Lucas Corlete Alves de Melo - NUSP: 13676461; Jean Carlos Pereira Cassiano - NUSP: 138640008
* Course: SCC0607 - Estrutura de Dados III
* Professor: Cristina Dutra de Aguiar
* Project: Trabalho Introdutório, 1 e 2 de ED3
* Description: Este trabalho tem como objetivo armazenar dados em um arquivo binário bem como desenvolver funcionalidades para a 
* manipulação desses dados. Novas funcionalidades serão adicionadas conforme o avançar da disciplina.
*/

#include <stdio.h>
#include "registeropertions.h"

/**
 * @brief Abertura de arquivo apenas para leitura
 * 
 * @param nomeArquivo 
 * @return arquivo aberto 
 */
FILE *abrirArquivoLeitura(const char *nomeArquivo){
    FILE* arquivo;

    arquivo = fopen(nomeArquivo, "rb");

    if(arquivo == NULL){
        printf("Falha no processamento do arquivo.\n");
        return NULL;
    }
    
    return arquivo; // Ponteiro no começo do arquivo
}

/**
 * @brief Abertura de arquivo apenas para escrita
 * 
 * @param nomeArquivo 
 * @return arquivo aberto 
 */
FILE *abrirArquivoEscrita(const char *nomeArquivo){
    FILE* arquivo;

    arquivo = fopen(nomeArquivo, "wb+");

    if(arquivo == NULL){
        printf("Falha no processamento do arquivo.\n");
        return NULL;
    }
    
    return arquivo; // Ponteiro no começo do arquivo
}

/**
 * @brief Remoção de caracteres do tipo LIXO de uma string
 * 
 * @param str 
 * @param tamanho 
 */
void removeLixo(char *str, int tamanho) {
    // Encontrar a posição do primeiro caractere de lixo ('$')
    int pos = 0;
    while (pos < tamanho && str[pos] != '$') {
        pos++;
    }
    strncpy(str, str, pos);
    str[pos] = '\0';
}

/**
 * @brief Função para fechamento de arquivo, verificando se é nulo ou não.
 * 
 * @param arquivo 
 */
void fecharArquivo(FILE *arquivo){
    if (arquivo != NULL) {
        fclose(arquivo);
    }
}

/**
 * @brief Remove as aspas de uma string, modificando a string original para excluir esses caracteres
 * 
 * @param str 
 */
void removeAspas(char *str) {
    int len = strlen(str);
    int j = 0;

    // Percorre a string original
    for (int i = 0; i < len; i++) {
        // Copia apenas os caracteres que não são aspas
        if (str[i] != '"') {
            str[j] = str[i];
            j++;
        }
    }

    // Adiciona o caractere nulo ao final da nova string
    str[j] = '\0';
}

/**
 * @brief Aloca memória para um novo Registro, incializando seus campos
 * 
 * @return Registro* 
 */
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

/**
 * @brief Aloca memória para um novo Cabeçalho, incializando seus campos
 * 
 * @return Cabecalho* 
 */
Cabecalho* inicializarCabecalho(){
    Cabecalho* cabecalho = malloc(sizeof(Cabecalho));
    cabecalho->status = '0'; 
    cabecalho->proxRRN = 0;   
    cabecalho->nroTecnologias = 0;  
    cabecalho->nroParesTecnologias = 0;
    
    return cabecalho;
}

/*
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
*/

/**
 * @brief Preenche o restante do registro com LIXO após os campos fixos e variáveis já preenchidos.
 * 
 * @param arquivo 
 * @param registro 
 */
void preenche_lixo(FILE* arquivo, Registro* registro){
    //variavel auxliar para definir o tamanho do lixo no registro atual
    int aux = (TAM_REGISTRO - (TAM_REGISTRO_FIXO + registro->tecnologiaOrigem.tamanho + registro->tecnologiaDestino.tamanho));
    
    char trash[aux];

    for(int i=0; i<aux; i++){
        trash[i] = LIXO;
    }

    fwrite(trash, sizeof(char), aux, arquivo);
}

/**
 * @brief Escreve um registro em um binário. Cada campo do registro é escrito no arquivo de acordo com seu tipo e tamanho,
 *  e o restante do registro é preenchido com LIXO para manter um tamanho fixo.
 * 
 * @param arquivo 
 * @param registro 
 */
void escreverRegistro(FILE *arquivo, Registro *registro){
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


/**
 * @brief Realiza a leitura do cabeçalho contendo registros.
 * 
 * @param arquivo 
 * @param c 
 * @return int 
 */
int lerCabecalho(FILE* arquivo, Cabecalho* c){

    if(fread(&(c->status), sizeof(char), 1, arquivo) == 0)
        return 0; //fim do arquivo

    fread(&c->proxRRN, sizeof(int), 1, arquivo);
    fread(&c->nroTecnologias, sizeof(int), 1, arquivo);
    fread(&c->nroParesTecnologias, sizeof(int), 1, arquivo);

    return 1; //leitura bem sucedida
}

/**
 * @brief escrever o cabeçalho contendo registros.
 * 
 * @param arquivo 
 * @param c 
 */
void escreverCabecalho(FILE* arquivo, Cabecalho* c){
    fwrite(&(c->status), sizeof(char), 1, arquivo);
    fwrite(&(c->proxRRN), sizeof(int), 1, arquivo);
    fwrite(&(c->nroTecnologias), sizeof(int), 1, arquivo);
    fwrite(&(c->nroParesTecnologias), sizeof(int), 1, arquivo);
}

/**
 * @brief Atualiza o cabeçalho com base nas informações de um registro atalizado.
 * 
 * @param registro 
 * @param cabecalho 
 */
void atualizaCabecalho(Registro* registro, Cabecalho* cabecalho){
    if(registro->tecnologiaDestino.tamanho != 0 && registro->tecnologiaOrigem.tamanho != 0)
        cabecalho->nroParesTecnologias++;
    cabecalho->proxRRN++;
}

/**
 * @brief Imprime na saída padrão as informações contidas em um registro, 
 * de modo que formata a saída de acordo com as especificações do campo e imprime "NULO" se o campo não estiver preenchido.
 * 
 * @param registro 
 */
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

