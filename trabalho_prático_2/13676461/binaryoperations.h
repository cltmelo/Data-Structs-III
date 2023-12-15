/*
* Name: Lucas Corlete Alves de Melo - NUSP: 13676461; Jean Carlos Pereira Cassiano - NUSP: 138640008
* Course: SCC0607 - Estrutura de Dados III
* Professor: Cristina Dutra de Aguiar
* Project: Trabalho Introdutório, 1 e 2 de ED3
* Description: Este trabalho tem como objetivo armazenar dados em um arquivo binário bem como desenvolver funcionalidades para a 
* manipulação desses dados. Novas funcionalidades serão adicionadas conforme o avançar da disciplina.
*/

#ifndef BINARYOPERATIONS_H_INCLUDED
#define BINARYOPERATIONS_H_INCLUDED

#define GLOBAL 256 //Só uma varável global aleatória para os arrays de nome de arquivo
/*
OBS:
Os valores nulos nos campos inteiros de tamanho fixo devem ser devem ser
representados pelo valor -1.
Os valores nulos nos campos de tamanho variável devem ser manipulados da
seguinte forma: o indicador de tamanho deve armazenar o valor zero.
*/

/*
==============================================================================================================
Funionalidades requeridas:
    1. Create Table: Cria uma tabela com os campos especificados com base em um .csv e gera de saída um .bin
    2. Select From: Imprime todos os registros de um .bin
    3. Select Where: Imprime todos os registros que contém os campo(s) especificado(s)
    4. Select RRN: Imprime o registro de determiando RRN
    5. btreeCreateTable: Indexa a chave de busca definida sobre o arquivo de dados
    6. btreeSelect: Recupera dados de todos os registros de um arquivo de dados.
    7. InsertInto: Insere novos registros no arquivo de dados.
    8. CriaGraph
    9. CriaGraphT
    10. TecnologiasGerada
    11. EhConexo
    12. BuscaCaminho
==============================================================================================================
*/

void criaTable();
void LerBIN();
// void selectWhere(); //OBS: Precisa ser devidamente corrigida, e como não a usamos neste trabalho, deixamos comentada!
void buscarRRN();
void btreeCreateTable();
void btreeSelect(); //NOTE: trazer como correção para a funcionalidade [3] em outro momento!
void InsertInto();
void CriaGraph();
void CriaGraphT();
void TecnologiasGerada();
void FortementeConexo();
void BuscaCaminho();

#endif // BINARYOPERATIONS_H_INCLUDED