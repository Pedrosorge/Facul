#ifndef SINTATICAL_ANALIZER 
#define SINTATICAL_ANALIZER
#include <stdio.h>
#include "../error_handler/error_handler.h"

extern char buffer_sinal[500];
char buffer_anterior[500];
int token_devolvido;
extern FILE *arq_read, *sintatical_errors;

void define_object_file(FILE * arq);

int PROXIMO_ANALIZER(FILE *arq);
void ANTERIOR_ANALIZER(FILE *arq);

void programa();                // Regra 1
void bloco();                   // Regra 2 
void parte_dec_variavel();      // Regra 3
void dec_variavel();            // Regra 4
void lista_identificadores();   // Regra 5
void parte_dec_sub_rotina();    // Regra 6
void dec_procedimento();        // Regra 7
void dec_funcao();              // Regra 8
void param_formais();           // Regra 9
void sec_param_formais();       // Regra 10
void com_composto();            // Regra 11
void comando();                 // Regra 12
void com_sem_rotulo();          // Regra 13
void atribuicao();              // Regra 14
void cham_procedimento();       // Regra 15
void com_condicional();         // Regra 16
void com_repetitivo();          // Regra 17
void lista_expressoes();        // Regra 18
void expressao();               // Regra 19
void relacao();                 // Regra 20
void exprecao_simples();        // Regra 21
void termo();                   // Regra 22
void fator();                   // Regra 23
void variavel();                // Regra 24
void cham_funcao();             // Regra 25

// Não sei se voi fazer esssas
/*
26. <número>
27. <digito> 
28. <identificador>  <-essa eu já fiz
29. <letra> 
*/



void identificador();           // Regra 28


#endif