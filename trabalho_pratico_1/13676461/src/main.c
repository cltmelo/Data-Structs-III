#include <stdio.h>
#include <string.h>
#include "../include/registros.h"

int main(){
    int a = 10;
    int b = 20;


    int c = somar(a, b);
    printf("%d", c);
    return 0;
}