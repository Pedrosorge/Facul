#include "./error_handler.h"


void ERRO(FILE *arq,int err_code, int line_num){

    switch (err_code)
    {
        case LEXICAL_ERROR_BAD_FORMATTING:  
            fprintf(arq, "ERRO DE LEXICO! Palavra mal formatada na linha : %d\n", line_num);
            break;
        case LEXICAL_ERROR_UNKNOW_CHAR:  
            fprintf(arq, "ERRO DE LEXICO! Caractere desconhecida na linha : %d\n", line_num);
            break;
        case SINTATICAL_ERROR_TOKEN_NOT_FOUND:  
            fprintf(arq, "ERRO DE SINTÁTICO! Token esperado não encontrada : %d\n", line_num);
            break;
        case SINTATICAL_ERROR_RESERVED_NOT_FOUND:  
            fprintf(arq, "ERRO DE SINTÁTICO! Palavra reservada esperada não encontrada : %d\n", line_num);
            break;
        case SINTATICAL_ERROR_OPERATOR_NOT_FOUND:  
            fprintf(arq, "ERRO DE SINTÁTICO! Operador esperado não encontrado : %d\n", line_num);
            break;
        case SINTATICAL_ERROR_IDENTIFICATOR_NOT_FOUND:  
            fprintf(arq, "ERRO DE SINTÁTICO! Identificador esperado não encontrado : %d\n", line_num);
            break;
        default:
            fprintf(arq,"ERRO DESCONHECIDO! Linha %d.\n", line_num);
            break;
    }

}

