#ifndef ERROR_HANDLER
#define ERROR_HANDLER
#include <stdio.h>

// Códigos de erro:
#define LEXICAL_ERROR_BAD_FORMATTING 1
#define LEXICAL_ERROR_UNKNOW_CHAR 2


void ERRO(FILE *arq, int err_code, int line_num);


#endif