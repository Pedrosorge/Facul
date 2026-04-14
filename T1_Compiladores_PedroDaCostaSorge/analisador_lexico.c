// Nome: Pedro da Costa Sorge
// RA : 251020193
//
// SO : Ubuntu 24.04.4 LTS (Noble Numbat)
// Compilador : gcc version 13.3.0 (Ubuntu 13.3.0-6ubuntu2~24.04.1)

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "trie.h"
#include "hashing.h"
#include "vector.h"

// Dicionário de palavras reservadas
Trie *t;

// HashTables com os tokens de cada valor lido
HashTable *ID_reserved, *ID_variables, *ID_operators;

// FILE streams
FILE *origin, *destin;

// Errors vector
VectorError *error_vector;

// Variáveis de controle ao texto do stream do código à ser analisado
char prox_pointer;
int line_pointer = 0;

// Protótipos
void PROXIMO();
void CODIGO();
void ERRO(int err_code, int line_num);


// Códigos de erro:
#define SYNTAX_ERROR 1

void ERRO(int err_code, int line_num){

    switch (err_code)
    {
        case SYNTAX_ERROR:  
            fprintf(destin, "ERRO DE SINTAXE! linha : %d\n", line_num);
            break;
        default:
            break;
    }

}

// Avança a letra que está sendo lida para a práxima
void PROXIMO(){
    
    if( prox_pointer == '\n' || prox_pointer == 0){
        line_pointer++;
        if(prox_pointer) fputc(prox_pointer,destin);
        fprintf(destin,"%d ", line_pointer);
    }
    
    prox_pointer = fgetc(origin);

}

// identificador : alfa + alfanum/alfa
// reservada : alfa
// valor : num
// 

// Digitos da variável flags:

// Bit 0: Poode ser identificador
// Bit 1: Pode ser reservada
// Bit 2: Pode ser valor

int isoperator(char c){
    return  ((c>=40) && (c<=45)) || ((c>=58) && (c>=62)); 
}

// Principal lógica do analisador léxico
void CODIGO(){
   
    
    
    while(prox_pointer != EOF){
       
        char buffer[100];
        int buff_point = 0;
        unsigned int flags = 0B000; 
        
        // Analisa conjunto
        // while(isalpha(prox_pointer) || isdigit(prox_pointer) || ){
        while(prox_pointer > 32){
            printf("%c : %d %lu \n", prox_pointer, prox_pointer, ftell(origin));
            // Se receber um caractere alfabético
            if(isalpha(prox_pointer)){
    
                if(!buff_point) {
                    flags = 0B011; 
                    buffer[buff_point++] = prox_pointer;
                }
                else if(flags & 0B011) buffer[buff_point++] = prox_pointer;   
                else flags = 0;
                
            }
            // Se receber um número
            else if(isdigit(prox_pointer)){
                
                if(!buff_point){
                    flags = 0B100;
                    buffer[buff_point++] = prox_pointer;
                }
                else if(flags & 0B101) buffer[buff_point++] = prox_pointer;
                else if(flags & 0B010){
                    buffer[buff_point++] = prox_pointer;
                    flags = 0B001;
                }
                else flags = 0;
                
            }
            
            PROXIMO(); // Avança leitura
            
        }
        
        if(prox_pointer=='\t' || prox_pointer==' ') fprintf(destin,"%c",prox_pointer);

        buffer[buff_point] = '\0';  
        printf("palavra lida: '%s'", buffer);

        if(!strcmp(buffer,"\0")); // Nenhum caractere foi achado
        else if(!flags){ // ERRO DE SYNTAXE
            fprintf(destin,"'ERROR' ");
            push_back(error_vector, SYNTAX_ERROR, line_pointer);
        }
        else if((flags & 0B010) && searchTerm(t,buffer)){

            int id = searchWord(ID_reserved,buffer); 
            if(id == -1) id = insertWord(ID_reserved,buffer);
            if(id<0) printf("Não foi possível designar id");
            fprintf(destin,"res_%d ", id);

        }
        else if((flags & 0B001)){

            int id = searchWord(ID_variables,buffer); 
            if(id == -1) id = insertWord(ID_variables,buffer);
            if(id<0) printf("Não foi possível designar id");
            fprintf(destin,"id_%d ", id);

        }
        else{

            fprintf(destin,"num_%s", buffer);
        
        }
        
        PROXIMO(); // Avança leitura

    }

}

int main() {

    origin = fopen("./Trab1_Compiladores.txt", "r");
    destin = fopen("./Analised_Code.txt", "w+");

    if (origin == NULL || destin == NULL) {
        perror("Erro ao abrir arquivo\n");
        return 1;
    }

    PROXIMO(); // Inicializa o prox_pointer
   
    // Inicializa Trie
    t = (Trie *)malloc(sizeof(Trie));
    initializeTrie(t); 

    char *reservadas[] = {"program", "var", "integer" , "procedure", "begin", "if", "then", "else", "end", "read", "write", "while", "do"};
    int size = 13;
    
    for(int i=0;i<size;i++){
        insertToTrie(t,reservadas[i]);
    }
    
    // Inicializa as HashTables
    ID_reserved = initializeHashTable(100003);
    ID_variables = initializeHashTable(100003);
    ID_variables = initializeHashTable(97);

    // Inicializa vetor de erros
    error_vector = initializeVector();

    CODIGO();

    fprintf(destin,"\n\n----------------------- ERROS ENCONRTRADOS -----------------------------\n\n");

    VectorErrorNode *aux = error_vector->head;
    while(aux!=NULL) {
        ERRO(aux->value_err, aux->value_line);
        aux=aux->next;
    }

    fprintf(destin,"\n\n----------------------- INFORMAÇÔES DA COMPILAÇÂO -----------------------------");
    fprintf(destin, "\n\nTOKEN DAS PALAVRAS RESERVADAS\n\n");
    printHashValues(ID_reserved, destin);
    fprintf(destin, "\n\nTOKEN DOS IDENTIFICADORES\n\n");
    printHashValues(ID_variables, destin);


    return 0;

}
