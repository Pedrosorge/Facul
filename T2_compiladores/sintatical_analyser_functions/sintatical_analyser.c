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

    // Verifica <parte de declarações de sub-rotinas> (OPCIONAL)
    parte_dec_sub_rotina();     

    // Verifica <comando composto>
    com_composto(); 

}

// FEITO !!!
// Desenvolve símbolo não terminal <comando composto> (ATUALIZAR ÁREAS DE ERRO)
void com_composto() {
    char expected_token[50];

    // Verifica 'begin'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
    sprintf(expected_token,"res_%d", searchWord(ID_reserved,"begin"));
    if(strcmp(buffer_sinal,expected_token)){ ERRO(sintatical_errors,0,0); return; }

    do {
        // Verifica <comando>
        comando();
    
        // Verifica ';'
        if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
        sprintf(expected_token,"op_%d", searchWord(ID_operators,";"));

    } while(!strcmp(expected_token,buffer_sinal));
    ANTERIOR_ANALIZER();

    // Verifica 'end'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
    sprintf(expected_token,"res_%d", searchWord(ID_reserved,"end"));
    if(strcmp(buffer_sinal,expected_token)){ ERRO(sintatical_errors,0,0); return; }

}

// FEITO !!!
// Desenvolve símbolo não terminal <comando> (ATUALIZAR ÁREAS DE ERRO)
void comando() {
    char expected_token[50];

    // Verifica <numero>
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
    if(!strncmp(buffer_sinal,"num_",4)) {
        // Verifica ':'
        if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
        sprintf(expected_token,"op_%d", searchWord(ID_operators,";"));
        if(strcmp(expected_token,buffer_sinal)) { ERRO(sintatical_errors,0,0); return; }
    }
    else ANTERIOR_ANALIZER(arq_read);

    // Verifica <comando sem rótulo>
    com_sem_rotulo();

}


// Desenvolve símbolo não terminal <comando sem rótulo> (ATUALIZAR ÁREAS DE ERRO)
void com_sem_rotulo() {
    
}

// FEITO
// Desenvolve símbolo não terminal <parte de declarações de sub-rotinas> (ATUALIZAR ÁREAS DE ERRO)
void parte_dec_sub_rotina() {
    char expected_token[50];

    do {
        // Verifica 'procedure'
        if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
        sprintf(expected_token,"res_%d", searchWord(ID_reserved,"procedure"));
        if(!strcmp(expected_token,buffer_sinal)){
            ANTERIOR_ANALIZER(arq_read);
            dec_procedimento();

            // Verifica ';'
            if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
            sprintf(expected_token,"op_%d", searchWord(ID_operators,";"));
            if(strcmp(expected_token,buffer_sinal)) { ERRO(sintatical_errors,0,0); return; }
            continue;
        }
    
        // Verifica 'function'
        if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
        sprintf(expected_token,"res_%d", searchWord(ID_reserved,"procedure"));
        if(!strcmp(expected_token,buffer_sinal)){
            ANTERIOR_ANALIZER(arq_read);
            dec_procedimento();
            
            // Verifica ';'
            if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
            sprintf(expected_token,"op_%d", searchWord(ID_operators,";"));
            if(strcmp(expected_token,buffer_sinal)) { ERRO(sintatical_errors,0,0); return; }
            continue;
        }

        break;

    } while( 1 );

}

// FEITO !!!
// Desenvolve símbolo não terminal <declaração de função> (ATUALIZAR ÁREAS DE ERRO)
void dec_funcao() {
    char expected_token[50];

    // Verifica 'procedure'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
    sprintf(expected_token,"res_%d", searchWord(ID_reserved,"function"));
    if(strcmp(expected_token,buffer_sinal)) { ERRO(sintatical_errors,0,0); return; }

    // Verifica <identificador>
    identificador();
    
    // Verifica <parâmetros formais> (OPCIONAL)
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
    sprintf(expected_token,"op_%d", searchWord(ID_operators,"("));
    if(!strcmp(expected_token,buffer_sinal)) { 
        ANTERIOR_ANALIZER(arq_read);
        param_formais();
    }

    // Verifica ';'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
    sprintf(expected_token,"op_%d", searchWord(ID_operators,";"));
    if(strcmp(expected_token,buffer_sinal)) { ERRO(sintatical_errors,0,0); return; }

    // Verifica <bloco>
    bloco();  

}

// FEITO !!!
// Desenvolve símbolo não terminal <declaração de procedimento> (ATUALIZAR ÁREAS DE ERRO)
void dec_procedimento() {
    char expected_token[50];
   
    // Verifica 'function'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
    sprintf(expected_token,"res_%d", searchWord(ID_reserved,"function"));
    if(strcmp(expected_token,buffer_sinal)) { ERRO(sintatical_errors,0,0); return; }

    // Verifica 'identificador'
    identificador();

    // Verifica <parâmetros formais> (OPCIONAL)
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
    sprintf(expected_token,"op_%d", searchWord(ID_operators,"("));
    if(!strcmp(expected_token,buffer_sinal)) { 
        ANTERIOR_ANALIZER(arq_read);
        param_formais();
    }

    // Verifica ':'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
    sprintf(expected_token,"op_%d", searchWord(ID_operators,":"));
    if(strcmp(expected_token,buffer_sinal)) { ERRO(sintatical_errors,0,0); return; }

    // Verifica <identificador>
    identificador();

   // Verifica ';'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
    sprintf(expected_token,"op_%d", searchWord(ID_operators,";"));
    if(strcmp(expected_token,buffer_sinal)) { ERRO(sintatical_errors,0,0); return; }

    // Verifica <bloco>
    bloco();  

}

// FEITO !!!
// Desenvolve símbolo não terminal <parte de declarações de variáveis> (ATUALIZAR ÁREAS DE ERRO)
void param_formais() {
    char expected_token[50];

    // Verifica '('
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
    sprintf(expected_token, "op_%d", searchWord(ID_operators,"("));
    if(strcmp(expected_token,buffer_sinal)) { ERRO(sintatical_errors,0,0); return; }

    
    do{
        // Verifica <seção de parametros formais>
        sec_param_formais();

        // Verifica ';'
        if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
        sprintf(expected_token, "op_%d", searchWord(ID_operators,";"));

    } while(!strcmp(expected_token,buffer_sinal));
    
    ANTERIOR_ANALIZER(arq_read);

    // Verifica ')'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
    sprintf(expected_token, "op_%d", searchWord(ID_operators,")"));
    if(strcmp(expected_token,buffer_sinal)) { ERRO(sintatical_errors,0,0); return; }
 
}

// FEITO !!!
// Desenvolve símbolo não terminal <seção de parâmetros formais> (ATUALIZAR ÁREAS DE ERRO)
void sec_param_formais() {
    char expected_token[50];
    
    // Verifica 'var' (OPCIONAL)
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
    sprintf(expected_token, "res_%d", searchWord(ID_reserved,"var"));
    if(strcmp(expected_token,buffer_sinal)) { 
        ANTERIOR_ANALIZER(arq_read);
    }

    // Verifica <lista de identificadores>
    lista_identificadores();

    // Verifica ':'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; }
    sprintf(expected_token,"op_%d", searchWord(ID_operators,":"));
    if(strcmp(expected_token,buffer_sinal)) { ERRO(sintatical_errors,0,0); return; }

    // Veriifica <identificador>
    identificador();

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
// Desenvolve símbolo não terminal <declaração de variáveis> (ATUALIZAR ÁREAS DE ERRO)
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
// Desenvolve símbolo não terminal <lista de identificadores> (ATUALIZAR ÁREAS DE ERRO)
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

// 
// Desenvolve símbolo não terminal <variável> (ATUALIZAR ÁREAS DE ERRO)
void variavel() { 
    // Verifica <identificador>
    identificador()
}

// FEITO !!!
// Verifica <identificador> (ATUALIZAR ÁREAS DE ERRO)
void identificador(){
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; };
    if(strncmp(buffer_sinal,"id_",3) != 0) { ERRO(sintatical_errors,0,0); return; };
}

// FEITO !!!
// Verifica <número> (ATUALIZAR ÁREAS DE ERRO)
void numero() {
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,0); return; };
    if(strncmp(buffer_sinal,"num_",4) != 0) { ERRO(sintatical_errors,0,0); return; };
}