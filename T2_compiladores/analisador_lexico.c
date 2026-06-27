// Nome: Pedro da Costa Sorge
// RA : 251020193
//
// SO : Ubuntu 24.04.4 LTS (Noble Numbat)
// Compilador : gcc version 13.3.0 (Ubuntu 13.3.0-6ubuntu2~24.04.1)

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "./data_structures/hashing.h"
#include "./data_structures/trie.h"
#include "./data_structures/vector.h"
#include "./sintatical_analyser_functions/sintatical_analyser.h"
#include "./symbol_tables/symbol_tables.h"
#include "./error_handler/error_handler.h"

// FILE streams
FILE *origin, *destin, *codes_only, *sintatical_errors;

// Errors vector
VectorError *error_vector;

// Variáveis de controle ao texto do stream do código à ser analisado
char prox_pointer;
int line_pointer = 0;

// Protótipos
void PROXIMO();
void CODIGO();

// Avança a letra que está sendo lida para a práxima
void PROXIMO(){
    
    if( prox_pointer == '\n' || prox_pointer == 0){
        line_pointer++;
        if(prox_pointer) {
            fputc(prox_pointer,destin);
            fprintf(codes_only,"jump_line ");
        }
        fprintf(destin,"%d\t", line_pointer);
    }
    
    prox_pointer = tolower(fgetc(origin));

}

// identificador : alfa + alfanum/alfa
// reservada : alfa
// valor : num
// operadores : op || op + op || div

// verifica se o caractere é um operador
int isoperator(char c){
    return  (((c>=40) && (c<=46)) || ((c>=58) && (c<=62))); 
}

// Principal lógica do analisador léxico
void CODIGO(){
   
    
    while(prox_pointer != EOF){
        
        char buffer[100];
        int buff_point = 0;
        unsigned int flags = 0B0000; 
        
        // --------------------------
        // Digitos da variável flags:
        // --------------------------
        // Bit 0: Pode ser identificador
        // Bit 1: Pode ser reservada
        // Bit 2: Pode ser número
        // Bit 3: Pode ser operador
        
        // Analisa conjunto
        int interruption = 1;
        int unk_char = 0;
        while(prox_pointer > 32 && interruption){
            // Se receber um caractere alfabético
            if(isalpha(prox_pointer)){
    
                if(!buff_point) // Se o buffer tiver vazio
                    flags = 0B0011;
                else if(!(flags & 0B0011)) // Se o que estiver no buffer não pode ser identificador ou palavra reservada 
                    flags = 0;
                
            }
            // Se receber um número
            else if(isdigit(prox_pointer)){
                
                if(!buff_point) // Se o buffer estiver vazio
                    flags = 0B0100;
                else if(flags & 0B0010) // Se o que tá no buffer pudesse ser uma palavra reservada
                    flags = 0B0001;
                else if(!(flags & 0B0101)) // Só entra caso o que tá no buffer não pode ser numero nem reservada
                    flags = 0;
            
            }
            // Se receber um operador
            else if(isoperator(prox_pointer)){
                
                if(!buff_point) // Se o buffer tiver vazio
                {
                    flags = 0B1000;
                    if(prox_pointer == '<'){
                        buffer[buff_point++] = prox_pointer;
                        PROXIMO();
                        if(prox_pointer == '=' || prox_pointer == '>') interruption = 0;
                        else break;
                    }
                    else if(prox_pointer == '>' || prox_pointer == ':'){
                        buffer[buff_point++] = prox_pointer;
                        PROXIMO();
                        if(prox_pointer == '=') interruption = 0;
                        else break;
                    }
                    interruption = 0;
                }
                else
                    break;

            }
            else{
                printf("Caractere inválido: %c\n", prox_pointer);
                fprintf(destin,"'ERROR' ");
                push_back(error_vector, "nada",LEXICAL_ERROR_UNKNOW_CHAR, line_pointer);
                unk_char=1;
                flags=0;
            }
            
            buffer[buff_point++] = prox_pointer;
            PROXIMO(); // Avança leitura
            
        }
        
        if(prox_pointer=='\t' || prox_pointer==' ') fprintf(destin,"%c",prox_pointer);

        buffer[buff_point] = '\0';  

        if(!strcmp(buffer,"\0")); // Nenhum caractere foi achado
        else if(unk_char); //
        else if(!flags){ // ERRO DE SYNTAXE
            printf("palavra : %s não reconecida\n" , buffer);
            fprintf(destin,"'ERROR' ");
            push_back(error_vector,"nada", LEXICAL_ERROR_BAD_FORMATTING, line_pointer);
        }
        else if(flags & 0B1000){

            int id = searchWord(ID_operators,buffer); 
            if(id == -1) id = insertWord(ID_operators,buffer);
            if(id<0) printf("Não foi possível designar id");
            fprintf(destin,"op_%d ", id);
            fprintf(codes_only,"op_%d ", id);
            continue;

        }
        else if((flags & 0B0010) && searchTerm(t,buffer)){

            int id = searchWord(ID_reserved,buffer); 
            if(id == -1) id = insertWord(ID_reserved,buffer);
            if(id<0) printf("Não foi possível designar id");
            fprintf(destin,"res_%d ", id);
            fprintf(codes_only,"res_%d ", id);

        }
        else if(flags & 0B0001){

            int id = searchWord(ID_variables,buffer); 
            if(id == -1) id = insertWord(ID_variables,buffer);
            if(id<0) printf("Não foi possível designar id");
            fprintf(destin,"id_%d ", id);
            fprintf(codes_only,"id_%d ", id);

        }
        else{

            fprintf(destin,"num_%s ", buffer);
            fprintf(codes_only,"num_%s ", buffer);
        
        }
        
        interruption = 1;
        if(prox_pointer > 32) continue;
        PROXIMO(); // Avança leitura

    }

}

int main() {

    origin = fopen("./Trab1_Compiladores.txt", "r");
    destin = fopen("./Analysed_Code.txt", "w+");
    codes_only = fopen("./Codes_Only.txt", "w+");
    sintatical_errors = fopen("./Sintatical_Errors.txt", "w+");

    if (origin == NULL || destin == NULL || codes_only == NULL) {
        perror("Erro ao abrir arquivo\n");
        return 1;
    }

    PROXIMO(); // Inicializa o prox_pointer

    // Inicializa vetor de erros
    error_vector = initializeVector();

    initialize_hashtables();
    initialize_reserved_words_trie();

    // Inicio do analizador léxico
    CODIGO();
    printf("Final da análise léxica!\n");

    // Inicio do analizador sintático
    fseek(codes_only,0,SEEK_SET);
    define_object_file(codes_only);
    programa();
    printf("Final da análise sintatica!\n");

    fprintf(destin,"\n\n----------------------- ERROS ENCONRTRADOS -----------------------------\n\n");

    VectorErrorNode *aux = error_vector->head;
    if(aux == NULL){
        fputs("Nenhum erro de sintaxe encontrado", destin);
    }
    while(aux!=NULL) {
        ERRO(destin, aux->value_err, aux->value_line);
        aux=aux->next;
    }

    fprintf(destin,"\n\n----------------------- INFORMAÇÔES DA COMPILAÇÂO -----------------------------");
    fprintf(destin, "\n\nTOKEN DAS PALAVRAS RESERVADAS\n\n");
    printHashValues(ID_reserved, destin);
    fprintf(destin, "\n\nTOKEN DOS IDENTIFICADORES\n\n");
    printHashValues(ID_variables, destin);
    fprintf(destin, "\n\nTOKEN DOS OPERADORES\n\n");
    printHashValues(ID_operators, destin);

    // Desalocando estruturas
    free_hashtables();
    free_reserved_words_trie();
    freeVector(error_vector);    


    
    fseek(codes_only,0,SEEK_SET);
    
    fclose(codes_only);
    fclose(origin);
    fclose(destin);    
    

    return 0;

}
