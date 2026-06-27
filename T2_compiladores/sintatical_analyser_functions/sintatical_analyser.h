#ifndef SINTATICAL_ANALIZER 
#define SINTATICAL_ANALIZER
#include <stdio.h>
#include "../error_handler/error_handler.h"

#define MAX_HISTORIC_CAPACITY 100

extern char buffer_sinal[500]; // Buffee onde os tokens lidos são armazendos
extern long historico_posicoes[MAX_HISTORIC_CAPACITY]; // Armazena as posições de leitura anteriores
extern long historico_linhas[MAX_HISTORIC_CAPACITY]; // Armazena os números das linhas de leitura anteriores
extern int wri_hist;  // Indice de escrita no histórico
extern int line_counter; // Contador de linhas

extern FILE *arq_read, *sintatical_errors;

void define_object_file(FILE * arq);

int PROXIMO_ANALIZER(FILE *arq);
void ANTERIOR_ANALIZER(FILE *arq);

void programa();                // Regra 1
void bloco();                   // Regra 2 
void parte_dec_variaveis();      // Regra 3
void dec_variaveis();            // Regra 4
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
void expressao_simples();        // Regra 21
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

void numero();                  // Regra 26
void identificador();           // Regra 28


#endif