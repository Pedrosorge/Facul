#include "sintatical_analyser.h"
#include <stdio.h>
#include <string.h>
#include "../data_structures/hashing.h"
#include "../symbol_tables/symbol_tables.h"
#include "../error_handler/error_handler.h"

// ============================================================
// Variáveis globais do analisador
// ============================================================

FILE *arq_read = NULL;
char buffer_sinal[500];
long historico_posicoes[MAX_HISTORIC_CAPACITY];
long historico_linhas[MAX_HISTORIC_CAPACITY];
int  wri_hist     = 0;
int  line_counter = 1;

// Define o arquivo de tokens a ser analisado
void define_object_file(FILE *arq){
    arq_read = arq;
}

// Avança para o próximo token; retorna 1 se leu, 0 se chegou ao fim
int PROXIMO_ANALIZER(FILE *arq){

    historico_posicoes[wri_hist] = ftell(arq);
    historico_linhas[wri_hist]   = line_counter;
    wri_hist = (wri_hist + 1) % MAX_HISTORIC_CAPACITY;

    if(fscanf(arq,"%499s ",buffer_sinal) != EOF){
        while(!strcmp(buffer_sinal,"jump_line")){
            line_counter++;
            if(fscanf(arq,"%499s ",buffer_sinal) == EOF) return 0;
        }
        return 1;
    }
    return 0;
}

// Volta um token (desfaz o último PROXIMO_ANALIZER)
void ANTERIOR_ANALIZER(FILE *arq){
    wri_hist = (wri_hist - 1 + MAX_HISTORIC_CAPACITY) % MAX_HISTORIC_CAPACITY;
    fseek(arq, historico_posicoes[wri_hist], SEEK_SET);
    line_counter = historico_linhas[wri_hist];
}

// ============================================================
// 1. <programa> ::= program <identificador> ; <bloco> .
// ============================================================
void programa(){

    char expected_token[50];

    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,SINTATICAL_ERROR_TOKEN_NOT_FOUND,line_counter); return; }

    // Verifica 'program'
    sprintf(expected_token,"res_%d",searchWord(ID_reserved,"program"));
    if(strcmp(buffer_sinal,expected_token)) { ERRO(sintatical_errors,SINTATICAL_ERROR_RESERVED_NOT_FOUND,line_counter); return; }

    // Verifica <identificador>
    identificador();

    // Verifica ';'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,SINTATICAL_ERROR_TOKEN_NOT_FOUND,line_counter); return; }
    sprintf(expected_token,"op_%d",searchWord(ID_operators,";"));
    if(strcmp(buffer_sinal,expected_token)) { ERRO(sintatical_errors,SINTATICAL_ERROR_OPERATOR_NOT_FOUND,line_counter); return; }

    // Verifica <bloco>
    bloco();

    // Verifica '.'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,SINTATICAL_ERROR_TOKEN_NOT_FOUND,line_counter); return; }
    sprintf(expected_token,"op_%d",searchWord(ID_operators,"."));
    if(strcmp(buffer_sinal,expected_token)) { ERRO(sintatical_errors,SINTATICAL_ERROR_OPERATOR_NOT_FOUND,line_counter); return; }

}

// ============================================================
// 2. <bloco> ::= [<parte de declarações de variáveis>]
//                [<parte de declarações de sub-rotinas>]
//                <comando composto>
// ============================================================
void bloco(){

    char expected_token[50];

    // Verifica opcionalmente 'var'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,SINTATICAL_ERROR_TOKEN_NOT_FOUND,line_counter); return; }
    sprintf(expected_token,"res_%d",searchWord(ID_reserved,"var"));
    if(!strcmp(buffer_sinal,expected_token)){
        ANTERIOR_ANALIZER(arq_read);
        parte_dec_variaveis();
    }
    else ANTERIOR_ANALIZER(arq_read);

    // Verifica opcionalmente 'procedure' ou 'function'
    if(PROXIMO_ANALIZER(arq_read)){
        char fun_token[50], proc_token[50];
        sprintf(fun_token,"res_%d",searchWord(ID_reserved,"function"));
        sprintf(proc_token,"res_%d",searchWord(ID_reserved,"procedure"));

        if(!strcmp(buffer_sinal,fun_token) || !strcmp(buffer_sinal,proc_token)){
            ANTERIOR_ANALIZER(arq_read);
            parte_dec_sub_rotina();
        }
        else ANTERIOR_ANALIZER(arq_read);
    }

    // Verifica <comando composto>
    com_composto();

}

// ============================================================
// 3. <parte de declarações de variáveis> ::=
//       var <declaração de variáveis> {; <declaração de variáveis>} ;
// ============================================================
void parte_dec_variaveis(){

    char expected_token[50];

    // Verifica 'var'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,SINTATICAL_ERROR_TOKEN_NOT_FOUND,line_counter); return; }
    sprintf(expected_token,"res_%d",searchWord(ID_reserved,"var"));
    if(strcmp(buffer_sinal,expected_token)) { ERRO(sintatical_errors,SINTATICAL_ERROR_RESERVED_NOT_FOUND,line_counter); return; }

    do {
        // Verifica <declaração de variáveis>
        dec_variaveis();

        // Verifica ';' obrigatório
        if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,SINTATICAL_ERROR_TOKEN_NOT_FOUND,line_counter); return; }
        sprintf(expected_token,"op_%d",searchWord(ID_operators,";"));
        if(strcmp(buffer_sinal,expected_token)) { ERRO(sintatical_errors,SINTATICAL_ERROR_OPERATOR_NOT_FOUND,line_counter); return; }

        // Lookahead: ainda há mais declarações?
        if(!PROXIMO_ANALIZER(arq_read)) return;
        if(!strncmp(buffer_sinal,"id_",3)){
            ANTERIOR_ANALIZER(arq_read);
            continue;
        }
        // Não é id_: ';' era o terminador, devolve token
        ANTERIOR_ANALIZER(arq_read);
        break;

    } while(1);

}

// ============================================================
// 4. <declaração de variáveis> ::= <lista de identificadores> : <tipo>
// ============================================================
void dec_variaveis(){

    char expected_token[50];

    // Verifica <lista de identificadores>
    lista_identificadores();

    // Verifica ':'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
    sprintf(expected_token,"op_%d",searchWord(ID_operators,":"));
    if(strcmp(buffer_sinal,expected_token)) { ERRO(sintatical_errors,0,line_counter); return; }

    // Verifica tipo (integer)
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
    sprintf(expected_token,"res_%d",searchWord(ID_reserved,"integer"));
    if(!strcmp(buffer_sinal,expected_token)) return;

    // Verifica tipo (boolean)
    sprintf(expected_token,"res_%d",searchWord(ID_reserved,"boolean"));
    if(!strcmp(buffer_sinal,expected_token)) return;

    // adicionar mais tipos caso haja ... (mesma estrutura que anteriormente)

    ERRO(sintatical_errors,0,line_counter);
    return;

}

// ============================================================
// 5. <lista de identificadores> ::= <identificador> {, <identificador>}
// ============================================================
void lista_identificadores(){

    char expected_token[50];

    do {
        // Verifica <identificador>
        identificador();

        // Verifica ','
        if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
        sprintf(expected_token,"op_%d",searchWord(ID_operators,","));

    } while(!strcmp(buffer_sinal,expected_token));
    ANTERIOR_ANALIZER(arq_read);

}

// ============================================================
// 6. <parte de declarações de sub-rotinas> ::=
//       { <declaração de procedimento>; | <declaração de função>; }
// ============================================================
void parte_dec_sub_rotina(){

    char expected_token[50], proc_token[50], fun_token[50];
    sprintf(proc_token,"res_%d",searchWord(ID_reserved,"procedure"));
    sprintf(fun_token,"res_%d",searchWord(ID_reserved,"function"));

    do {
        if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }

        // Verifica 'procedure'
        if(!strcmp(buffer_sinal,proc_token)){
            ANTERIOR_ANALIZER(arq_read);
            dec_procedimento();
        }
        // Verifica 'function'
        else if(!strcmp(buffer_sinal,fun_token)){
            ANTERIOR_ANALIZER(arq_read);
            dec_funcao();
        }
        else {
            ANTERIOR_ANALIZER(arq_read);
            break;
        }

        // Verifica ';'
        if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
        sprintf(expected_token,"op_%d",searchWord(ID_operators,";"));
        if(strcmp(buffer_sinal,expected_token)) { ERRO(sintatical_errors,0,line_counter); return; }

    } while(1);

}

// ============================================================
// 7. <declaração de procedimento> ::=
//       procedure <identificador> [<parâmetros formais>] ; <bloco>
// ============================================================
void dec_procedimento(){

    char expected_token[50];

    // Verifica 'procedure'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
    sprintf(expected_token,"res_%d",searchWord(ID_reserved,"procedure"));
    if(strcmp(buffer_sinal,expected_token)) { ERRO(sintatical_errors,0,line_counter); return; }

    // Verifica <identificador>
    identificador();

    // Verifica opcionalmente <parâmetros formais>
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
    sprintf(expected_token,"op_%d",searchWord(ID_operators,"("));
    if(!strcmp(expected_token,buffer_sinal)){
        ANTERIOR_ANALIZER(arq_read);
        param_formais();
    }
    else ANTERIOR_ANALIZER(arq_read);

    // Verifica ';'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
    sprintf(expected_token,"op_%d",searchWord(ID_operators,";"));
    if(strcmp(buffer_sinal,expected_token)) { ERRO(sintatical_errors,0,line_counter); return; }

    // Verifica <bloco>
    bloco();

}

// ============================================================
// 8. <declaração de função> ::=
//       function <identificador> [<parâmetros formais>] : <identificador> ; <bloco>
// ============================================================
void dec_funcao(){

    char expected_token[50];

    // Verifica 'function'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
    sprintf(expected_token,"res_%d",searchWord(ID_reserved,"function"));
    if(strcmp(buffer_sinal,expected_token)) { ERRO(sintatical_errors,0,line_counter); return; }

    // Verifica <identificador> (nome da função)
    identificador();

    // Verifica opcionalmente <parâmetros formais>
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
    sprintf(expected_token,"op_%d",searchWord(ID_operators,"("));
    if(!strcmp(expected_token,buffer_sinal)){
        ANTERIOR_ANALIZER(arq_read);
        param_formais();
    }
    else ANTERIOR_ANALIZER(arq_read);

    // Verifica ':'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
    sprintf(expected_token,"op_%d",searchWord(ID_operators,":"));
    if(strcmp(buffer_sinal,expected_token)) { ERRO(sintatical_errors,0,line_counter); return; }

    // Verifica <identificador> (tipo de retorno)
    identificador();

    // Verifica ';'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
    sprintf(expected_token,"op_%d",searchWord(ID_operators,";"));
    if(strcmp(buffer_sinal,expected_token)) { ERRO(sintatical_errors,0,line_counter); return; }

    // Verifica <bloco>
    bloco();

}

// ============================================================
// 9. <parâmetros formais> ::=
//       ( <seção de parâmetros formais> {; <seção de parâmetros formais>} )
// ============================================================
void param_formais(){

    char expected_token[50];

    // Verifica '('
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
    sprintf(expected_token,"op_%d",searchWord(ID_operators,"("));
    if(strcmp(buffer_sinal,expected_token)) { ERRO(sintatical_errors,0,line_counter); return; }

    do {
        // Verifica <seção de parâmetros formais>
        sec_param_formais();

        // Verifica ';'
        if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
        sprintf(expected_token,"op_%d",searchWord(ID_operators,";"));

    } while(!strcmp(expected_token,buffer_sinal));
    ANTERIOR_ANALIZER(arq_read);

    // Verifica ')'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
    sprintf(expected_token,"op_%d",searchWord(ID_operators,")"));
    if(strcmp(buffer_sinal,expected_token)) { ERRO(sintatical_errors,0,line_counter); return; }

}

// ============================================================
// 10. <seção de parâmetros formais> ::=
//        [var] <lista de identificadores> : <identificador>
// ============================================================
void sec_param_formais(){

    char expected_token[50];

    // Verifica opcionalmente 'var'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
    sprintf(expected_token,"res_%d",searchWord(ID_reserved,"var"));
    if(strcmp(buffer_sinal,expected_token))
        ANTERIOR_ANALIZER(arq_read);

    // Verifica <lista de identificadores>
    lista_identificadores();

    // Verifica ':'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
    sprintf(expected_token,"op_%d",searchWord(ID_operators,":"));
    if(strcmp(buffer_sinal,expected_token)) { ERRO(sintatical_errors,0,line_counter); return; }

    // Verifica <identificador> (tipo)
    identificador();

}

// ============================================================
// 11. <comando composto> ::= begin <comando> {; <comando>} end
// ============================================================
void com_composto(){

    char expected_token[50];

    // Verifica 'begin'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,SINTATICAL_ERROR_TOKEN_NOT_FOUND,line_counter); return; }
    sprintf(expected_token,"res_%d",searchWord(ID_reserved,"begin"));
    if(strcmp(buffer_sinal,expected_token)) { ERRO(sintatical_errors,SINTATICAL_ERROR_RESERVED_NOT_FOUND,line_counter); return; }

    // Verifica se é 'end' imediatamente (bloco vazio)
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,SINTATICAL_ERROR_TOKEN_NOT_FOUND,line_counter); return; }
    sprintf(expected_token,"res_%d",searchWord(ID_reserved,"end"));
    if(!strcmp(buffer_sinal,expected_token)) return;
    ANTERIOR_ANALIZER(arq_read);

    do {
        // Verifica <comando>
        comando();

        // Verifica ';'
        if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,SINTATICAL_ERROR_TOKEN_NOT_FOUND,line_counter); return; }
        sprintf(expected_token,"op_%d",searchWord(ID_operators,";"));

        if(!strcmp(buffer_sinal,expected_token)){
            // Lookahead: próximo é 'end'?
            if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,SINTATICAL_ERROR_TOKEN_NOT_FOUND,line_counter); return; }
            char end_token[50];
            sprintf(end_token,"res_%d",searchWord(ID_reserved,"end"));
            if(!strcmp(buffer_sinal,end_token)) return;
            ANTERIOR_ANALIZER(arq_read);
        }
        else break;

    } while(1);

    // Verifica 'end'
    sprintf(expected_token,"res_%d",searchWord(ID_reserved,"end"));
    if(strcmp(buffer_sinal,expected_token)) { ERRO(sintatical_errors,SINTATICAL_ERROR_RESERVED_NOT_FOUND,line_counter); return; }

}

// ============================================================
// 12. <comando> ::= [<número> :] <comando sem rótulo>
// ============================================================
void comando(){

    char expected_token[50];

    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,SINTATICAL_ERROR_TOKEN_NOT_FOUND,line_counter); return; }

    // Verifica opcionalmente <número> ':'
    if(!strncmp(buffer_sinal,"num_",4)){
        if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,SINTATICAL_ERROR_TOKEN_NOT_FOUND,line_counter); return; }
        sprintf(expected_token,"op_%d",searchWord(ID_operators,":"));
        if(strcmp(buffer_sinal,expected_token)){
            ANTERIOR_ANALIZER(arq_read);
            ANTERIOR_ANALIZER(arq_read);
        }
    }
    else ANTERIOR_ANALIZER(arq_read);

    // Verifica <comando sem rótulo>
    com_sem_rotulo();

}

// ============================================================
// 13. <comando sem rótulo> ::= <atribuição> | <chamada de procedimento> |
//                              <comando composto> | <comando condicional> |
//                              <comando repetitivo>
// ============================================================
void com_sem_rotulo(){

    char expected_token[50];

    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }

    // Verifica se pode ser <comando composto>
    sprintf(expected_token,"res_%d",searchWord(ID_reserved,"begin"));
    if(!strcmp(expected_token,buffer_sinal)){
        ANTERIOR_ANALIZER(arq_read);
        com_composto();
        return;
    }

    // Verifica se pode ser <comando condicional>
    sprintf(expected_token,"res_%d",searchWord(ID_reserved,"if"));
    if(!strcmp(expected_token,buffer_sinal)){
        ANTERIOR_ANALIZER(arq_read);
        com_condicional();
        return;
    }

    // Verifica se pode ser <comando repetitivo>
    sprintf(expected_token,"res_%d",searchWord(ID_reserved,"while"));
    if(!strcmp(expected_token,buffer_sinal)){
        ANTERIOR_ANALIZER(arq_read);
        com_repetitivo();
        return;
    }

    // Verifica se pode ser <atribuição>
    if(!strncmp(buffer_sinal,"id_",3)){
        if(PROXIMO_ANALIZER(arq_read)){
            sprintf(expected_token,"op_%d",searchWord(ID_operators,":="));
            if(!strcmp(expected_token,buffer_sinal)){
                ANTERIOR_ANALIZER(arq_read);
                ANTERIOR_ANALIZER(arq_read);
                atribuicao();
                return;
            }
            ANTERIOR_ANALIZER(arq_read);
        }
        // Não é atribuição: é <chamada de procedimento>
        ANTERIOR_ANALIZER(arq_read);
        cham_procedimento();
        return;
    }

    // Verifica <chamada de procedimento> com palavra reservada (ex: read, write)
    if(!strncmp(buffer_sinal,"res_",4)){
        ANTERIOR_ANALIZER(arq_read);
        cham_procedimento();
        return;
    }

    ERRO(sintatical_errors,0,line_counter);
    ANTERIOR_ANALIZER(arq_read);

}

// ============================================================
// 14. <atribuição> ::= <variável> := <expressão>
// ============================================================
void atribuicao(){

    char expected_token[50];

    // Verifica <variável>
    variavel();

    // Verifica ':='
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
    sprintf(expected_token,"op_%d",searchWord(ID_operators,":="));
    if(strcmp(expected_token,buffer_sinal)) { ERRO(sintatical_errors,0,line_counter); return; }

    // Verifica <expressão>
    expressao();

}

// ============================================================
// 15. <chamada de procedimento> ::= <identificador> [( <lista de expressões> )]
// ============================================================
void cham_procedimento(){

    char expected_token[50];

    // Verifica <identificador>
    identificador();

    // Verifica se tem <lista de expressões>
    if(PROXIMO_ANALIZER(arq_read)){
        sprintf(expected_token,"op_%d",searchWord(ID_operators,"("));
        if(!strcmp(expected_token,buffer_sinal)){
            lista_expressoes();

            // Verifica ')'
            if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
            sprintf(expected_token,"op_%d",searchWord(ID_operators,")"));
            if(strcmp(expected_token,buffer_sinal)) { ERRO(sintatical_errors,0,line_counter); return; }
            return;
        }
        else ANTERIOR_ANALIZER(arq_read);
    }

}

// ============================================================
// 16. <comando condicional> ::=
//        if <expressão> then <comando sem rótulo> [else <comando sem rótulo>]
// ============================================================
void com_condicional(){

    char expected_token[50];

    // Verifica 'if'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
    sprintf(expected_token,"res_%d",searchWord(ID_reserved,"if"));
    if(strcmp(expected_token,buffer_sinal)) { ERRO(sintatical_errors,0,line_counter); return; }

    // Verifica <expressão>
    expressao();

    // Verifica 'then'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
    sprintf(expected_token,"res_%d",searchWord(ID_reserved,"then"));
    if(strcmp(expected_token,buffer_sinal)) { ERRO(sintatical_errors,0,line_counter); return; }

    // Verifica <comando sem rótulo>
    com_sem_rotulo();

    // Verifica se tem 'else'
    if(PROXIMO_ANALIZER(arq_read)){
        sprintf(expected_token,"res_%d",searchWord(ID_reserved,"else"));
        if(!strcmp(expected_token,buffer_sinal)){
            com_sem_rotulo();
        }
        else ANTERIOR_ANALIZER(arq_read);
    }

}

// ============================================================
// 17. <comando repetitivo> ::= while <expressão> do <comando sem rótulo>
// ============================================================
void com_repetitivo(){

    char expected_token[50];

    // Verifica 'while'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
    sprintf(expected_token,"res_%d",searchWord(ID_reserved,"while"));
    if(strcmp(expected_token,buffer_sinal)) { ERRO(sintatical_errors,0,line_counter); return; }

    // Verifica <expressão>
    expressao();

    // Verifica 'do'
    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
    sprintf(expected_token,"res_%d",searchWord(ID_reserved,"do"));
    if(strcmp(expected_token,buffer_sinal)) { ERRO(sintatical_errors,0,line_counter); return; }

    // Verifica <comando sem rótulo>
    com_sem_rotulo();

}

// ============================================================
// 18. <lista de expressões> ::= <expressão> {, <expressão>}
// ============================================================
void lista_expressoes(){

    char expected_token[50];

    do {
        // Verifica <expressão>
        expressao();

        // Verifica ','
        if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
        sprintf(expected_token,"op_%d",searchWord(ID_operators,","));

    } while(!strcmp(expected_token,buffer_sinal));
    ANTERIOR_ANALIZER(arq_read);

}

// ============================================================
// 19. <expressão> ::= <expressão simples> [<relação> <expressão simples>]
// ============================================================
void expressao(){

    // Verifica <expressão simples>
    expressao_simples();

    // Verifica opcionalmente [<relação> <expressão simples>]
    if(PROXIMO_ANALIZER(arq_read)){
        char expected_token[50];
        char relacoes[6][3] = {"<>","<",">","=","<=",">="};
        int i;
        for(i=0;i<6;i++){
            sprintf(expected_token,"op_%d",searchWord(ID_operators,relacoes[i]));
            if(!strcmp(buffer_sinal,expected_token)) break;
        }
        if(i==6){ ANTERIOR_ANALIZER(arq_read); return; }

        // Verifica <relação>
        relacao();

        // Verifica <expressão simples>
        expressao_simples();
    }

}

// ============================================================
// 20. <relação> ::= = | <> | < | <= | >= | >
// ============================================================
void relacao(){

    char expected_token[50];
    char relacoes[6][3] = {"<>","<",">","=","<=",">="};
    int i;
    for(i=0;i<6;i++){
        sprintf(expected_token,"op_%d",searchWord(ID_operators,relacoes[i]));
        if(!strcmp(buffer_sinal,expected_token)) return;
    }
    ERRO(sintatical_errors,0,line_counter);

}

// ============================================================
// 21. <expressão simples> ::= [+ | -] <termo> {(+ | - | or) <termo>}
// ============================================================
void expressao_simples(){

    char expected_token[50];

    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }

    // Verifica sinal opcional '+' ou '-'
    int tem_sinal = 0;
    sprintf(expected_token,"op_%d",searchWord(ID_operators,"+"));
    if(!strcmp(expected_token,buffer_sinal)) tem_sinal = 1;

    sprintf(expected_token,"op_%d",searchWord(ID_operators,"-"));
    if(!strcmp(expected_token,buffer_sinal)) tem_sinal = 1;

    if(tem_sinal){
        if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
    }
    else ANTERIOR_ANALIZER(arq_read);

    do {
        // Verifica <termo>
        termo();

        if(!PROXIMO_ANALIZER(arq_read)) return;

        // Verifica '+'
        sprintf(expected_token,"op_%d",searchWord(ID_operators,"+"));
        if(!strcmp(expected_token,buffer_sinal)) continue;

        // Verifica '-'
        sprintf(expected_token,"op_%d",searchWord(ID_operators,"-"));
        if(!strcmp(expected_token,buffer_sinal)) continue;

        // Verifica 'or'
        sprintf(expected_token,"res_%d",searchWord(ID_reserved,"or"));
        if(!strcmp(expected_token,buffer_sinal)) continue;

        ANTERIOR_ANALIZER(arq_read);
        break;

    } while(1);

}

// ============================================================
// 22. <termo> ::= <fator> {(* | div | and) <fator>}
// ============================================================
void termo(){

    char expected_token[50];

    do {
        // Verifica <fator>
        fator();

        if(!PROXIMO_ANALIZER(arq_read)) return;

        // Verifica '*'
        sprintf(expected_token,"op_%d",searchWord(ID_operators,"*"));
        if(!strcmp(expected_token,buffer_sinal)) continue;

        // Verifica 'div'
        sprintf(expected_token,"res_%d",searchWord(ID_reserved,"div"));
        if(!strcmp(expected_token,buffer_sinal)) continue;

        // Verifica 'and'
        sprintf(expected_token,"res_%d",searchWord(ID_reserved,"and"));
        if(!strcmp(expected_token,buffer_sinal)) continue;

        ANTERIOR_ANALIZER(arq_read);
        break;

    } while(1);

}

// ============================================================
// 23. <fator> ::= <variável> | <número> | <chamada de função> |
//                 ( <expressão> ) | not <fator>
// ============================================================
void fator(){

    char expected_token[50];

    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }

    // Verifica se é 'not'
    sprintf(expected_token,"res_%d",searchWord(ID_reserved,"not"));
    if(!strcmp(expected_token,buffer_sinal)){
        fator();
        return;
    }

    // Verifica se é '('
    sprintf(expected_token,"op_%d",searchWord(ID_operators,"("));
    if(!strcmp(expected_token,buffer_sinal)){
        expressao();

        if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
        sprintf(expected_token,"op_%d",searchWord(ID_operators,")"));
        if(strcmp(expected_token,buffer_sinal)) { ERRO(sintatical_errors,0,line_counter); return; }
        return;
    }

    // Verifica se é <número>
    if(!strncmp(buffer_sinal,"num_",4)) return;

    // Deve ser <variável> ou <chamada de função>
    ANTERIOR_ANALIZER(arq_read);
    cham_funcao();

}

// ============================================================
// 25. <chamada de função> ::= <identificador> [( <lista de expressões> )]
// ============================================================
void cham_funcao(){

    char expected_token[50];

    // Verifica <identificador>
    identificador();

    // Verifica se tem <lista de expressões>
    if(PROXIMO_ANALIZER(arq_read)){
        sprintf(expected_token,"op_%d",searchWord(ID_operators,"("));
        if(!strcmp(expected_token,buffer_sinal)){
            lista_expressoes();

            // Verifica ')'
            if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
            sprintf(expected_token,"op_%d",searchWord(ID_operators,")"));
            if(strcmp(expected_token,buffer_sinal)) { ERRO(sintatical_errors,0,line_counter); return; }
            return;
        }
        ANTERIOR_ANALIZER(arq_read);
    }

}

// ============================================================
// 24. <variável> ::= <identificador>
// ============================================================
void variavel(){
    identificador();
}

// ============================================================
// 28. <identificador>
// ============================================================
void identificador(){

    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,SINTATICAL_ERROR_TOKEN_NOT_FOUND,line_counter); return; }
    // Aceita id_ e res_ (para read/write tratados como procedimentos)
    if(strncmp(buffer_sinal,"id_",3) != 0 && strncmp(buffer_sinal,"res_",4) != 0)
        { ERRO(sintatical_errors,SINTATICAL_ERROR_IDENTIFICATOR_NOT_FOUND,line_counter); return; }

}

// ============================================================
// 26-27. <número>
// ============================================================
void numero(){

    if(!PROXIMO_ANALIZER(arq_read)) { ERRO(sintatical_errors,0,line_counter); return; }
    if(strncmp(buffer_sinal,"num_",4) != 0) { ERRO(sintatical_errors,0,line_counter); return; }

}
