#ifndef BINARYOPERATIONS_H_INCLUDED
#define BINARYOPERATIONS_H_INCLUDED

// #include "../includes/funcoesFornecidas.h"
// #include "../includes/registeropertions.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

/*
OBS:
Os valores nulos nos campos inteiros de tamanho fixo devem ser devem ser
representados pelo valor -1.
Os valores nulos nos campos de tamanho variável devem ser manipulados da
seguinte forma: o indicador de tamanho deve armazenar o valor zero.
*/

/*
-----------------------------------------------------------------------------------------------
Funionalidades requeridas:
    1. Create Table: Cria uma tabela com os campos especificados com base em um .csv e gera de saída um .bin
    2. Select From: Imprime todos os registros de um .bin
    3. Select Where: Imprime todos os registros que contém os campo(s) especificado(s)
    4. Select RRN: Imprime o registro de determiando RRN
-----------------------------------------------------------------------------------------------
*/

void criaTable();
void LerBIN();
void selectWhere();
void buscarRRN();
void btreeCreateTable();
void btreeSelect();
void InserctInto();
void CriaGraph();
void CriaGraphT();

#endif // BINARYOPERATIONS_H_INCLUDED