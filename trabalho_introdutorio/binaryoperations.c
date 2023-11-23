#include "binaryoperations.h"
#include "funcoesAuxiliares.h"

/*
----------------------------------------------------------------------------------------------------------------------------------
Funcionalidades do algoritmo: 
    1. Create Table (criaTable)
    2. Select From
    3. Select Where
    4. Select RRN (bucarRRN)
----------------------------------------------------------------------------------------------------------------------------------
*/


#define GLOBAL 256 //Só uma varável global aleatória para os arrays de nome de arquivo

//Funcionalidade 1
void criaTable(){
    char arq_csv[GLOBAL];
    char arq_bin[GLOBAL];
    scanf("%s", arq_csv);
    scanf("%s", arq_bin);

    FILE *csv = fopen(arq_csv, "r");
    FILE *bin = abrirArquivoEscrita(arq_bin);

    
    
    // método de armazenamento em memória primária para as tecnologias lidas
    Lista *lista = cria_lista();

    Registro *registro = inicializarRegistro();
    Cabecalho *cabecalho = inicializarCabecalho();
    char input[GLOBAL];
    //Pular o cabeçalho do .csv
    fgets(input, sizeof(input), csv);
    escreverCabecalho(bin, cabecalho);
    while (fgets(input, sizeof(input), csv) != NULL) {
        // Divide a string para struct registro. preenchendo os campos vazios com NULL
        registro = separaString(input);
        if((registro->tecnologiaDestino.tamanho) != 0) {
            insere_lista_final(lista, registro->tecnologiaDestino.string);
        }
        if(registro->tecnologiaOrigem.tamanho != 0){
            insere_lista_final(lista, registro->tecnologiaOrigem.string);
        }
        
        escreverRegistro(bin, registro);
        
        atualizaCabecalho(registro, cabecalho);

    }

    cabecalho->nroTecnologias = tamanho_lista(lista) -1;

    cabecalho->status = REMOVIDO; //atualiza o status do cabeçalho como logicamente removido

    fseek(bin, 0, SEEK_SET);
    escreverCabecalho(bin, cabecalho);

    //Fechando os arquivos de dados
    fecharArquivo(csv);
    fecharArquivo(bin);
    //imprimeLista(lista);
    //liberando memoria alocada (lista ligada)
    libera_lista(lista);

    //REQUISITO DO ENUNCIADO
    binarioNaTela(arq_bin);
}

//Funcionalidade 2
void LerBIN() {
    char arq_bin[GLOBAL];
    scanf("%s", arq_bin);

    FILE *arquivo = abrirArquivoLeitura(arq_bin);   

    Registro* registro = inicializarRegistro();
    Cabecalho* cabecalho = inicializarCabecalho();
    if(!lerCabecalho(arquivo, cabecalho)){
        printf("Registro Inexistente.\n");
        return;
    }
    
    while (fread(&(registro->removido), sizeof(char), 1, arquivo) == 1) {
        fread(&(registro->grupo), sizeof(int), 1, arquivo);
        fread(&(registro->popularidade), sizeof(int), 1, arquivo);
        fread(&(registro->peso), sizeof(int), 1, arquivo);
        
        fread(&(registro->tecnologiaOrigem.tamanho), sizeof(int), 1, arquivo);
        registro->tecnologiaOrigem.string = (char *)malloc(registro->tecnologiaOrigem.tamanho + 1);
        fread(registro->tecnologiaOrigem.string, registro->tecnologiaOrigem.tamanho, 1, arquivo);
        registro->tecnologiaOrigem.string[registro->tecnologiaOrigem.tamanho] = NULL_TERM;


        fread(&(registro->tecnologiaDestino.tamanho), sizeof(int), 1, arquivo);
        registro->tecnologiaDestino.string = (char *)malloc(registro->tecnologiaDestino.tamanho + 1);
        fread(registro->tecnologiaDestino.string, registro->tecnologiaDestino.tamanho, 1, arquivo);
        registro->tecnologiaDestino.string[registro->tecnologiaDestino.tamanho] = NULL_TERM;

        int tamRegistro = TAM_REGISTRO_FIXO + registro->tecnologiaDestino.tamanho + registro->tecnologiaOrigem.tamanho;
        char resto[TAM_REGISTRO-tamRegistro];
        fread(resto, 1, TAM_REGISTRO-tamRegistro, arquivo);
        if (registro->removido == '0'){
            printRegister(registro);
        }       
        
    }
    fecharArquivo(arquivo);
}

// Funcionalidade 3
void selectWhere(){
    char arq_bin[GLOBAL]; // Nome do arquivo binario
    char nomeCampo[GLOBAL]; // nome do campo a ser buscado
    int quantidade_busca; // Quantidade de busca;

    // Recebe o nome do arquivo de entrada e a quantidade de valores para buscar
    scanf("%s %d", arq_bin, &quantidade_busca);  

    // // abre o arquivo de entrada
    // FILE* bin = fopen(nome_bin, "rb");
    // // checa erros na abertura do arquivo
    // if(bin == NULL) {
    //     printf("Falha no processamento do arquivo.\n");
    //     return;
    // }
    FILE* bin = abrirArquivoLeitura(arq_bin);

    // le registro de cabecalho e vai ao primeiro RRN, retorna quaisquer erros
    Cabecalho cabecalho;
    int output = ler_header(bin, &cabecalho);
    if(output == 1) { ///alterar os retornos tb
        printf("Falha no processamento do arquivo.\n");
        // fclose(bin);
        fecharArquivo(bin);
        return;
    }
    
    // retorna erro caso 'status' do arquivo lido seja '0' (inconsistente)
    if(cabecalho.status == INCONSISTENTE) {
        printf("Falha no processamento do arquivo.\n");
        // fclose(bin);
        fecharArquivo(bin);
        return;
    }

    // declara variáveis a serem empregadas no loop de leitura
    char* tmp = malloc(GLOBAL *sizeof(char)); // valor temporario do campo a ser buscado
    char* valorBuscado; // ponteiro para armazenamento do campo buscado apos remocao de ""

    // repete o processo de busca para cada campo distinto a ser avaliado 
    for(int i = 0; i < quantidade_busca; i++){
        // Recebo o nome e valor do campo a serem buscados
        scanf("%s", nomeCampo);
        scanf("%s", tmp);

        valorBuscado = strtok(tmp, "\"");

        // variaveis de apoio
        Registro registro; // registro a ser devolvido
        char* valorAtual = malloc(GLOBAL * sizeof(char)); // valor do campo sendo lido no momento
        int RRN = 0; // valor do RRN do registro a ser lido
        int contBuscado = 0; // Quantidade de registros que satisfazem busca
        while(1){
            // vai a posicao do RRN especificado
            fseek(bin, byte_offset(RRN), SEEK_SET);
            int output = ler_campo(bin, &valorAtual, nomeCampo);
            if(output == 1) {
                // break com fim do arquivo
                break;
            }

            // Erro encontrado durante a busca no arquivo
            else if(output == (-1)) {
                printf("Falha no processamento do arquivo.");
                free(valorAtual); 
                free(tmp);
                // fclose(bin);
                fecharArquivo(bin);
                return;
            }
            
            // verifica se o registro atual satisfaz a busca
            if(strcmp(valorBuscado, valorAtual) == 0){
                contBuscado++;
                fseek(bin, byte_offset(RRN), SEEK_SET);
                int aux = ler_registro(bin, &registro); // lê registro atual ///////////////////////////////////////////////ACHEI A FUNCAO CAGADA
                if(aux) {
                    // break com fim do arquivo
                    break;
                }


                // imprime os dados contidos no registro lido, caso nao removido
                if(registro.removido != REMOVIDO)
                    printRegister(&registro);

                // libera as strings alocadas
                free(registro.tecnologiaOrigem.string);
                free(registro.tecnologiaDestino.string);
            }
            // Acresce para busca no proximo registro.
            RRN++;
        }
        if(contBuscado == 0){
            printf("Registro inexistente.\n");
        }
        free(valorAtual);
        
        int aux = quantidade_busca - 1;
        if(i < aux)
            fseek(bin, TAM_CABECALHO, SEEK_SET);
    }
    free(tmp);
    // fclose(bin); // Fechar arquivo
    fecharArquivo(bin);
}   


//Funcionalidade 4
void buscarRRN(){
    int RRN; // Quantidade de busca;
    char arq_bin[GLOBAL]; // Nome do arquivo binario

    // Recebe o nome do arquivo de entrada e a quantidade de valores para buscar
    scanf("%s %d", arq_bin, &RRN);

    FILE *arquivo = abrirArquivoLeitura(arq_bin);

    Cabecalho *cabecalho = inicializarCabecalho();
    Registro* registro = inicializarRegistro();   

    

    lerCabecalho(arquivo, cabecalho);
    long pos = byte_offset(RRN);
    fseek(arquivo, pos, SEEK_SET);
    if (fread(&(registro->removido), sizeof(char), 1, arquivo) != 0){
            if (registro->removido == NAO_REMOVIDO){
            fread(&registro->grupo, sizeof(int), 1, arquivo);
            fread(&registro->popularidade, sizeof(int), 1, arquivo);
            fread(&registro->peso, sizeof(int), 1, arquivo);

            fread(&registro->tecnologiaOrigem.tamanho, sizeof(int), 1, arquivo);
            registro->tecnologiaOrigem.string = (char *)malloc(registro->tecnologiaOrigem.tamanho + 1);
            fread(registro->tecnologiaOrigem.string, registro->tecnologiaOrigem.tamanho, 1, arquivo);
            registro->tecnologiaOrigem.string[registro->tecnologiaOrigem.tamanho] = '\0';

            fread(&registro->tecnologiaDestino.tamanho, sizeof(int), 1, arquivo);
            registro->tecnologiaDestino.string = (char *)malloc(registro->tecnologiaDestino.tamanho + 1);
            fread(registro->tecnologiaDestino.string, registro->tecnologiaDestino.tamanho, 1, arquivo);
            registro->tecnologiaDestino.string[registro->tecnologiaDestino.tamanho] = '\0';

            int tamRegistro = TAM_REGISTRO_FIXO + registro->tecnologiaDestino.tamanho + registro->tecnologiaOrigem.tamanho;
            char resto[TAM_REGISTRO-tamRegistro];
            fread(resto, 1, TAM_REGISTRO-tamRegistro, arquivo);  

            printRegister(registro);
        } else {
            printf("Registro inexistente.\n");
        }
    } else {
        printf("Registro inexistente.\n");
    }
    fecharArquivo(arquivo);
}


// Funcionalidade 5
void btreeCreateTable(){
    
}

// Funcionalidade 6 
void btreeSelect(){

}

// Funcionalidade 7
void InsertInto(){

}

/*
----------------------------------------------------------------------------------------------------------------------------------
*/