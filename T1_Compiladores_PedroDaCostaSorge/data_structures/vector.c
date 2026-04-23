#include <stdlib.h>
#include "vector.h"

// Inicializa vetor
VectorError* initializeVector(){
    return (VectorError *)calloc(1,sizeof(VectorError));
}

// Insere elemento no final do vetor 
void push_back(VectorError *vec,int err, int line){ 
    VectorErrorNode *aux = (VectorErrorNode *)malloc(sizeof(VectorErrorNode));

    aux->next = NULL;
    aux->value_err = err;
    aux->value_line = line;
    
    if(vec->head == NULL){
        vec->head = aux;
        vec->tail = aux;
        return;
    }

    vec->tail->next = aux;
    vec->tail = aux;
}

// Desaloca o vetor
void freeVector(VectorError *v){
    VectorErrorNode *aux = v->head;

    while(aux != NULL){
        VectorErrorNode *temp = aux;
        aux=aux->next;
        free(temp);
    }
    free(v);
}