/*
* Name: Lucas Corlete Alves de Melo - NUSP: 13676461; Jean Carlos Pereira Cassiano - NUSP: 138640008
* Course: SCC0607 - Estrutura de Dados III
* Professor: Cristina Dutra de Aguiar
* Project: Trabalho Introdutório, 1 e 2 de ED3
* Description: Este trabalho tem como objetivo armazenar dados em um arquivo binário bem como desenvolver funcionalidades para a 
* manipulação desses dados. Novas funcionalidades serão adicionadas conforme o avançar da disciplina.
*/

#include "binaryoperations.h"
#include "funcoesAuxiliares.h"
#include "btree.h"
#include "graphs.h"

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

    cabecalho->nroTecnologias = tamanho_lista(lista);

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
        if (registro->removido == NAO_REMOVIDO){
            printRegister(registro);
            
        }       
        
    }
    fecharArquivo(arquivo);
}

/*
// Funcionalidade 3
void selectWhere(){ // Pode ser deveras custosa em termos de disco, Jean, por causa da alocação dinâmica.
    char arq_bin[GLOBAL];
    char nomeCampo[GLOBAL];
    int quatidade_busca;
    scanf("%s %d", arq_bin, &quatidade_busca);
    FILE *bin = abrirArquivoLeitura(arq_bin);

    Cabecalho cabecalho;
    int output = lerCabecalho(bin, &cabecalho);

    if(output == 0){
        printf("Falha no processamento do arquivo.\n");
        fecharArquivo(bin);
        return;
    }

    if(cabecalho.status == NAO_REMOVIDO){
        printf("Falha no processamento do arquivo.\n");
        fecharArquivo(bin);
        return;
    }

    
    // char* tmp = malloc(GLOBAL *sizeof(char)); // auxilar temp do campo a ser buscado
    char tmp[GLOBAL];

    // repete o processo de busca para cada campo distinto a ser avaliado 
    for(int i = 0; i < quatidade_busca; i++){
        scanf("%s", nomeCampo);
        scanf("%s", tmp);
        
        scan_quote_string(tmp);

         // ponteiro para armazenamento do campo buscado depois da remocao das aspas

        
        char* actual_field = malloc(GLOBAL*sizeof(char)); // valor do campo sendo lido no momento (me enrolei com a estática, vai com a dinâmica mesmo)
        int contRRN = 0; // valor do RRN do registro a ser lido
        int contBuscado = 0; // Quantidade de registros que satisfazem busca
        int indexAux; // soh um auxilar p ver (contBuscado - [algo])

        Registro *registro = inicializarRegistro(); // registro a ser devolvido
        while(1){ //tu colocou while(0) e ai o loop não executava kkkkk
            fseek(bin, byte_offset(contRRN), SEEK_SET);
            //scan_quote_string(tmp);
            
            // verifica se o registro atual satisfaz a busca
            if(strcmp(tmp, actual_field) == 0){
                fseek(bin, byte_offset(contRRN), SEEK_SET);

                contBuscado++;

                if(lerRegistro(bin, &registro)) { //leitura do registro atual
                    break; //fim do arquivo
                }

                // imprime os dados contidos no registro lido
                if(registro->removido != REMOVIDO)
                    printRegister(&registro);

                // libera as strings alocadas
                free(registro->tecnologiaDestino.string);
                free(registro->tecnologiaOrigem.string);
            }
            contRRN++; //incremento do RRN para ir ao próximo registro
        }
        free(actual_field);

        if(contBuscado == 0){
            printf("Registro inexistente.\n");
        }

        indexAux = contBuscado - 1; //ata, era -1
        if(i < indexAux){
            fseek(bin, TAM_CABECALHO, SEEK_SET);
        }
    }
    //free(tmp); //pra que isso pae? utilizamos estática (acho qeu vai ser mais eficiente em termos de memória)
    fecharArquivo(bin); // Fechar arquivo
}
*/
//Funcionalidade 3
void selectWhere() {
    char arq_bin[GLOBAL];
    int qnt;
    int busca = 0;
    scanf("%s", arq_bin);
    scanf("%d", &qnt);
    FILE* bin = abrirArquivoLeitura(arq_bin);
    char campo[GLOBAL];
    char temp[GLOBAL];
    Cabecalho cabecalho;
    if(!lerCabecalho(bin, &cabecalho)){
        printf("Registro Inexistente.\n");
        return;
    }
    Registro *registro = inicializarRegistro();
    for (int i = 0; i < qnt; i++){
        busca = 0;
        fseek(bin, TAM_CABECALHO, SEEK_SET);
        scanf("%s%s", campo, temp);
        removeAspas(temp);
        int cmp;
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
            if (registro->removido == NAO_REMOVIDO){
                cmp = strcmp(campo, "nomeTecnologiaOrigem");
                if (cmp == 0){
                    if (strcmp(temp, registro->tecnologiaOrigem.string) == 0){
                        printRegister(registro);
                        busca++;
                    }
                }
                cmp = strcmp(campo, "nomeTecnologiaDestino");
                if (cmp == 0){
                    if (strcmp(temp, registro->tecnologiaDestino.string) == 0){
                        printRegister(registro);
                        busca++;
                    }
                }
                cmp = strcmp(campo, "popularidade");
                if (cmp == 0){
                    if (registro->popularidade == atoi(temp)){
                        printRegister(registro);
                        busca++;
                    }
                }
                cmp = strcmp(campo, "grupo");
                if (cmp == 0){
                    if (registro->grupo == atoi(temp)){
                        printRegister(registro);
                        busca++;
                    }
                }
                cmp = strcmp(campo, "peso");
                if (cmp == 0){
                    if (registro->peso == atoi(temp)){
                        printRegister(registro);
                        busca++;
                    }
                }
                
                
            }       
            
        }
        if (busca == 0){
            printf("Registro inexistente.\n");
        }
        


    }
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
                registro->tecnologiaOrigem.string[registro->tecnologiaOrigem.tamanho] = NULL_TERM;

                fread(&registro->tecnologiaDestino.tamanho, sizeof(int), 1, arquivo);
                registro->tecnologiaDestino.string = (char *)malloc(registro->tecnologiaDestino.tamanho + 1);
                fread(registro->tecnologiaDestino.string, registro->tecnologiaDestino.tamanho, 1, arquivo);
                registro->tecnologiaDestino.string[registro->tecnologiaDestino.tamanho] = NULL_TERM;

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

/**
 * @brief Funcionalidade 5: indexa a chave de busca definida sobre o arquivo de dados.
 * 
 */
void btreeCreateTable(){
    char arq_bin[GLOBAL];
    char arq_indice[GLOBAL];
    scanf("%s", arq_bin);
    scanf("%s", arq_indice);

    FILE *indice = abrirArquivoEscrita(arq_indice);
    // FILE *indice;
    // indice = fopen(arq_indice, "wb");
    if (indice == NULL) {
        // printf("Falha no processamento do arquivo.\n");
        return;
    }
    FILE *bin = abrirArquivoLeitura(arq_bin);
    // FILE *bin;
    // bin = fopen(arq_bin, "rb");
    if (bin == NULL) {
        // printf("Falha no processamento do arquivo.\n");
        return;
    }
    btree_header bHeader = criarArvoreB();
    escreve_btree_header(indice, &bHeader);
    Registro* registro = inicializarRegistro();
    Cabecalho* cabecalho = inicializarCabecalho();
    fseek(bin, 0, SEEK_SET); 
    // lerCabecalho(bin, cabecalho);
    if (!lerCabecalho(bin, cabecalho)) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    int RRN = 0;
    int chaves = 0;
    Node no = criaNode();
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
            char concat[registro->tecnologiaDestino.tamanho + registro->tecnologiaOrigem.tamanho];
            strcpy(concat, registro->tecnologiaOrigem.string);
            strcat(concat, registro->tecnologiaDestino.string);
            
            InserirNo(indice, concat, RRN, no, -1, &bHeader);
        }
        free(registro->tecnologiaOrigem.string);
        free(registro->tecnologiaDestino.string);
        RRN++;
    }
    bHeader.status = '1';
    escreve_btree_header(indice, &bHeader);
    fecharArquivo(bin);
    fecharArquivo(indice);
    binarioNaTela(arq_indice);
}

/**
 * @brief Funcionalidade 6: recupera dados de todos os registros de um arquivo de dados.
 */
void btreeSelect(){
    char arq_bin[GLOBAL];
    char arq_indice[GLOBAL];
    int qnt;
    int busca = 0;
    scanf("%s", arq_bin);
    scanf("%s", arq_indice);
    scanf("%d", &qnt);
    FILE* bin = abrirArquivoLeitura(arq_bin);
    FILE* indice = abrirArquivoLeitura(arq_indice);
    char campo[GLOBAL];
    char temp[GLOBAL];
    Cabecalho cabecalho;
    if(!lerCabecalho(bin, &cabecalho)){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    btree_header bHeader = LerHeader(indice);
    if (bHeader.status == '0'){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    Registro *registro = inicializarRegistro();
    for (int i = 0; i < qnt; i++){
        busca = 0;
        fseek(bin, TAM_CABECALHO, SEEK_SET);
        scanf("%s%s", campo, temp);
        removeAspas(temp);
        int cmp = strcmp(campo, "nomeTecnologiaOrigemDestino");
        if (cmp == 0){
            int aux = Busca(indice, temp, bHeader.noRaiz);
            if (aux == -1){
                printf("Registro inexistente.\n");
            } else {
                fseek(bin, byte_offset(aux), SEEK_SET);
                fread(&(registro->removido), sizeof(char), 1, bin);
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

                if (registro->removido == NAO_REMOVIDO){
                    printRegister(registro);
                } 
            }          
        } else {
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
                if (registro->removido == NAO_REMOVIDO){
                    cmp = strcmp(campo, "nomeTecnologiaOrigem");
                    if (cmp == 0){
                        if (strcmp(temp, registro->tecnologiaOrigem.string) == 0){
                            printRegister(registro);
                            busca++;
                        }
                    }
                    cmp = strcmp(campo, "nomeTecnologiaDestino");
                    if (cmp == 0){
                        if (strcmp(temp, registro->tecnologiaDestino.string) == 0){
                            printRegister(registro);
                            busca++;
                        }
                    }
                    cmp = strcmp(campo, "popularidade");
                    if (cmp == 0){
                        if (registro->popularidade == atoi(temp)){
                            printRegister(registro);
                            busca++;
                        }
                    }
                    cmp = strcmp(campo, "grupo");
                    if (cmp == 0){
                        if (registro->grupo == atoi(temp)){
                            printRegister(registro);
                            busca++;
                        }
                    }
                    cmp = strcmp(campo, "peso");
                    if (cmp == 0){
                        if (registro->peso == atoi(temp)){
                            printRegister(registro);
                            busca++;
                        }
                    }
                    
                    
                }       
                
            }
            if (busca == 0){
                printf("Registro inexistente.\n");
            }
        }
        


    }
    fecharArquivo(bin);
    fecharArquivo(indice);
}

/**
 * @brief Funcionalidade 7: insere novos registros no arquivo de dados, a qual deve ser realizada no final do arquivo.
 * 
 */
void InsertInto(){
    int quantidade;
    char arq_bin[GLOBAL];
    char arq_indice[GLOBAL];
    scanf("%s", arq_bin);
    scanf("%s", arq_indice);
    scanf("%d", &quantidade);
    FILE *indice  = fopen(arq_indice, "rb+");
    if (indice == NULL) {
        printf("Falha no processamento do arquivo.\n");
        fclose(indice);
        return;
    }
    // FILE *bin = abrirArquivoLeitura(arq_bin);
    FILE *bin;
    bin = fopen(arq_bin, "rb+");
    if (bin == NULL) {
        printf("Falha no processamento do arquivo.\n");
        fclose(bin);
        return;
    }
    Registro *registro = inicializarRegistro();
    Cabecalho *cabecalho = inicializarCabecalho();
    Lista *lista = cria_lista();

    if(!lerCabecalho(bin, cabecalho)){
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
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
        if (registro->removido == NAO_REMOVIDO){
            if((registro->tecnologiaDestino.tamanho) != 0) {
                insere_lista_final(lista, registro->tecnologiaDestino.string);
            }
            if(registro->tecnologiaOrigem.tamanho != 0){
                insere_lista_final(lista, registro->tecnologiaOrigem.string);
            }
        }  
    }
    // fecharArquivo(bin);
    // bin = fopen(arq_bin, "rb+");
    // fseek(bin, byte_offset(cabecalho->proxRRN), SEEK_SET);
    btree_header bHeader = LerHeader(indice);
    if (bHeader.status == '0') {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    Node no = criaNode();
    char nomeTecnologiaOrigem[100], grupo[100], popularidade[100], nomeTecnologiaDestino[100], peso[100];
    for (int i = 0; i < quantidade; i++){
        scanf("\n%[^,], %[^,], %[^,], %[^,], %s", nomeTecnologiaOrigem, grupo, popularidade, nomeTecnologiaDestino, peso);
        if (strcmp(nomeTecnologiaOrigem, "NULO") != 0){
            strcpy(registro->tecnologiaOrigem.string, nomeTecnologiaOrigem);
            registro->tecnologiaOrigem.tamanho = strlen(nomeTecnologiaOrigem);
        } else {
            registro->tecnologiaOrigem.string[0] = NULL_TERM;
            registro->tecnologiaOrigem.tamanho = 0;
        }
        if (strcmp(nomeTecnologiaDestino, "NULO") != 0){
            strcpy(registro->tecnologiaDestino.string, nomeTecnologiaDestino);
            registro->tecnologiaDestino.tamanho = strlen(nomeTecnologiaDestino);
        } else {
            registro->tecnologiaDestino.string[0] = NULL_TERM;
            registro->tecnologiaDestino.tamanho = 0;
        }
        if (strcmp(popularidade, "NULO") != 0){
            registro->popularidade = atoi(popularidade);
        } else {
            registro->popularidade = -1;
        }
        if (strcmp(peso, "NULO") != 0){
            registro->peso = atoi(peso);
        } else {
            registro->peso = -1;
        }
        if (strcmp(grupo, "NULO") != 0){
            registro->grupo = atoi(grupo);
        } else {
            registro->grupo = -1;
        }
        if((registro->tecnologiaDestino.tamanho) != 0) {
            insere_lista_final(lista, registro->tecnologiaDestino.string);
        }
        if((registro->tecnologiaOrigem.tamanho) != 0){
            insere_lista_final(lista, registro->tecnologiaOrigem.string);
        }
        registro->removido = NAO_REMOVIDO;
        escreverRegistro(bin, registro);
        if (registro->removido == NAO_REMOVIDO && registro->tecnologiaDestino.tamanho != 0 && registro->tecnologiaOrigem.tamanho != 0){
            char concat[registro->tecnologiaDestino.tamanho + registro->tecnologiaOrigem.tamanho];
            strcpy(concat, registro->tecnologiaOrigem.string);
            strcat(concat, registro->tecnologiaDestino.string);
            InserirNo(indice, concat, cabecalho->proxRRN, no, -1, &bHeader);
        }
        atualizaCabecalho(registro, cabecalho);
    }

    cabecalho->nroTecnologias = tamanho_lista(lista);
    cabecalho->status = REMOVIDO; //atualiza o status do cabeçalho como logicamente removido
    fseek(bin, 0, SEEK_SET);
    escreverCabecalho(bin, cabecalho);
    fecharArquivo(bin);
    //Fechando os arquivos de dados
    //imprimeLista(lista);
    //liberando memoria alocada (lista ligada)
    libera_lista(lista);
    //REQUISITO DO ENUNCIADO
    binarioNaTela(arq_bin);
    bHeader.status = '1'; //CONSISTENTE
    escreve_btree_header(indice, &bHeader);
    fecharArquivo(indice);
    binarioNaTela(arq_indice);

}

//funcionalidade 8
void CriaGraph(){
    char arq_bin[GLOBAL];

    scanf("%s", arq_bin);

    FILE* bin = abrirArquivoLeitura(arq_bin);
    
    
    Lista *lista = cria_lista();

    Grafo* gr = geraGrafo(bin, lista);
    
    imprime_Grafo(gr, lista);
    libera_Grafo(gr);
    fecharArquivo(bin);


}


//funcionalidade9
void CriaGraphT(){
    char arq_bin[GLOBAL];

    scanf("%s", arq_bin);

    FILE* bin = abrirArquivoLeitura(arq_bin);
    
    
    Lista *lista = cria_lista();

    Grafo* gr = geraGrafo(bin, lista);
    
    imprime_GrafoT(gr, lista);
    libera_Grafo(gr);
    fecharArquivo(bin);


}

//funcionalidade10
void TecnologiasGerada(){
    char arq_bin[GLOBAL];

    scanf("%s", arq_bin);

    FILE* bin = abrirArquivoLeitura(arq_bin);
    
    
    Lista *lista = cria_lista();

    Grafo* gr = geraGrafo(bin, lista);
    char tec[GLOBAL];
    int quantidade;
    scanf("%d", &quantidade);
    for (int i = 0; i < quantidade; i++){
        scanf("%s", tec);
        removeAspas(tec);
        imprimirTecGerada(gr, lista, tec);
    }

    
    libera_Grafo(gr);
    fecharArquivo(bin);

}

//funcionalidade11
void FortementeConexo(){
    // char arquivo_binario[] = "tecnologias.bin";  // Nome do arquivo binário

    // FILE* bin = fopen(arquivo_binario, "rb");

    // if (bin == NULL) {
    //     printf("Erro ao abrir o arquivo %s\n", arquivo_binario);
    //     return 1;
    // }

    char arq_bin[GLOBAL];

    scanf("%s", arq_bin);

    FILE* bin = abrirArquivoLeitura(arq_bin);

    Lista* lista = cria_lista();
    Grafo* gr = geraGrafo(bin, lista);

    if (gr == NULL) {
        printf("Erro ao criar o grafo a partir do arquivo %s\n", arq_bin);
        fclose(bin);
        return;
    }

    componentesFortementeConexos(gr);

    libera_Grafo(gr);
    fclose(bin);
}

//funcionalidade12
void BuscaCaminho(){}