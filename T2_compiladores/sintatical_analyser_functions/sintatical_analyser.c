#include "sintatical_analyser.h"
#include <stdio.h>
#include <string.h>
#include "../data_structures/hashing.h"
#include "../symbol_tables/symbol_tables.h"
#include "../error_handler/error_handler.h"

FILE *arq_read = NULL; // Ponteiro para arquivo de leitura dos tokens
char buffer_sinal[500]; // Buffer onde sao armazenados os tokens
char buffer_anterior[500]; // Armazena o token que acabou de ser substituído
int token_devolvido = 0;

// Difine arquivo com os tokens que serão verificados
void define_object_file(FILE * arq){
    arq_read = arq;
}

// Avança token a ser analizado
int PROXIMO_ANALIZER(FILE *arq){

    if(token_devolvido){
        strcpy(buffer_sinal,buffer_anterior);
        token_devolvido = 0;
        return 1;
    }

    strcpy(buffer_anterior,buffer_sinal);

    if(fscanf(arq,"%499s ", buffer_sinal) != EOF) return 1;
    return 0;
}

// Volta um token a ser lido
void ANTERIOR_ANALIZER(FILE *arq){
    token_devolvido = 1;
}

// FEITO!!!
// Desenvolve símbolo inicial <programa> (ATUALIZAR ÁREAS DE ERRO)
void programa(){

    char expected_token[50];
    if(!PROXIMO_ANALIZER(arq_read)) {ERRO(sintatical_errors,0,0); return;};

    // Verifica 'program' 
    sprintf(expected_token, "res_%d", searchWord(ID_reserved,"program"));
    if(strcmp(buffer_sinal,expected_token)) { ERRO(sintatical_errors,0,0); return; } 

    //Verifica <identificador>
    identificador();
    
    // Verifica ';'
    PROXIMO_ANALIZER(arq_read);
    sprintf(expected_token,"op_%d",searchWord(ID_operators,";"));
    if(strcmp(buffer_sinal,expected_token)) { ERRO(sintatical_errors,0,0); return; }
    
    // Verifica <bloco>
    bloco();
    
    // Verifica '.'
    PROXIMO_ANALIZER(arq_read);
    sprintf(expected_token,"op_%d",searchWord(ID_operators,"."));
    if(strcmp(buffer_sinal,expected_token)) { ERRO(sintatical_errors,0,0); return; }
    
}



// Desenvolve símbolo não terminal <bloco> (ATUALIZAR ÁREAS DE ERRO)
void bloco(){ 

    char expected_token[50];

    // dec_rotulos(); // OPCIONAL (não achei no documento dela)
    
    // Verifica se é 'var' (OPCIONAL)
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
    sprintf(expected_token,"res_%d", searchWord(ID_reserved,"var"));
    if(!strcmp(buffer_sinal,expected_token)){
        ANTERIOR_ANALIZER(arq_read);
        parte_dec_variaveis(); 
    }

    dec_sub_rot(); // OPCIONAL

    comando_composto(); // OPCIONAL

}

// FEITO !!!
// Desenvolve símbolo não terminal <parte de declarações de variáveis> (ATUALIZAR ÁREAS DE ERRO)
void parte_dec_variaveis(){

    char expected_token[50];

    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }

    // Verifica 'var'
    sprintf(expected_token,"res_%d", searchWord(ID_reserved,"var"));
    if(strcmp(buffer_sinal,expected_token)){ ERRO(sintatical_errors,0,0); return; }

    // Verifica <declaração de variáveis>
    dec_variaveis();

    // Verifica ';'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
    sprintf(expected_token,"op_%d",searchWord(ID_operators,";"));
    if(strcmp(buffer_sinal,expected_token)) { ERRO(sintatical_errors,0,0); return; }

}

// FEITO !!!
// Desenvolve símbolo não terminal <declaração de variáveis>
void dec_variaveis() {

    char expected_token[50];

    // <lista de identificadores>
    lista_identificadores();

    // Verifica ':'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return;}
    sprintf(expected_token,"op_%d", searchWord(ID_operators, ":"));
    if(strcmp(buffer_sinal, expected_token)) { ERRO(sintatical_errors,0,0); return; }

    // Verifica tipo de variável (integer ou boolean)
    
    // (integer)
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
    sprintf(expected_token,"res_%d",searchWord(ID_reserved,"integer"));
    if(!strcmp(buffer_sinal,expected_token)) return;
    
    // (boolean)
    sprintf(expected_token,"res_%d",searchWord(ID_reserved,"boolean"));
    if(!strcmp(buffer_sinal,expected_token))  return;

    // adicionar mais tipos caso haja ... (mesma estrutura que anteriormente)

    ERRO(sintatical_errors,0,0); 
    return;
}

// FEITO !!!
// Desenvolve símbolo não terminal <lista de identificadores>
void lista_identificadores() {

    char expected_token[50];

    do {
        // Verifica <identificador>
        identificador();
        
        // Verifica ','
        if(!PROXIMO_ANALIZER(arq_read)){ ERRO(sintatical_errors,0,0); return; }
        sprintf(expected_token,"op_%d", searchWord(ID_operators,","));

    } while(!strcmp(buffer_sinal, expected_token));
    ANTERIOR_ANALIZER(arq_read);

}

// FEITO !!!
// Verifica identificador (ATUALIZAR ÁREAS DE ERRO)
void identificador(){
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; };
    if(strncmp(buffer_sinal,"id_",3) != 0) { ERRO(sintatical_errors,0,0); return; };
}
