// Nome: Pedro da Costa Sorge
// RA : 251020193
//
// SO : Ubuntu 24.04.4 LTS (Noble Numbat)
// Compilador : gcc version 13.3.0 (Ubuntu 13.3.0-6ubuntu2~24.04.1)

#include <stdio.h>
#include <stdlib.h>
#include "trie.h"

// FILE streams
FILE *origin, *destin;

// Variáveis de controle ao texto do stream do código à ser analisado
char prox_pointer;
int line_pointer = 0;

// Protótipos
int PROXIMO();
void CODIGO();
void ERRO(int err_code);

// Avança a letra que está sendo lida para a práxima
int PROXIMO(){
    
    prox_pointer = fgetc(origin);
    
    while(prox_pointer == '\n'){ 
        line_pointer++;
        fputc(prox_pointer,destin);
        fprintf(destin,"%d", line_pointer);
        
        prox_pointer = fgetc(origin);
    }

}

void CODIGO(){



}


int main() {

    origin = fopen("./Trab1_Compiladores.txt", "r");
    destin = fopen("./Analised_Code.txt", "w+");

    if (origin == NULL || destin == NULL) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    PROXIMO();

    Trie *t;
    initializeTrie(t); 

    char *reservadas[] = {"program", "var", "integer" , "procedure", "begin", "end", "if", "else", "then"};

    for(int i=0;i<)
    
    return 0;

}
