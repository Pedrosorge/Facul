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
        default:
            fprintf(arq,"ERRO DESCONHECIDO! Linha %d.", line_num);
            break;
    }

}

