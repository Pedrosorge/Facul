#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "hashing.h"

// Inicializa hashtable
HashTable* initializeHashTable(int size) {
    HashTable *h = (HashTable *)malloc(sizeof(HashTable));
    h->palavras = (char **)calloc(size, sizeof(char*));
    h->size = size;
    return h;
}

// Encontra um valor númerico para determinada palavra
int hashFunction(int size, char *palavra, int try_num) {
    long long aux = 0;
    for (int i = 0; palavra[i] != '\0'; i++) {
        aux = (aux * CONST_PRIME + palavra[i]) % size;
    }
    return (aux + try_num) % size;
}

// Procura palavra na tabela hashing
int searchWord(HashTable *h, char *word) {
    for (int i = 0; i < h->size; i++) {
        int id = hashFunction(h->size, word, i);
        if (h->palavras[id] == NULL) return -1; 
        if (strcmp(h->palavras[id], word) == 0) return id;
    }
    return -1;
}

// Insere palavra na tabela hashing
int insertWord(HashTable *h, char *word) {
    int id = searchWord(h, word);
    if (id != -1) return id; 

    for (int i = 0; i < h->size; i++) {
        int possivel_id = hashFunction(h->size, word, i);
        if (h->palavras[possivel_id] == NULL) {
            h->palavras[possivel_id] = strdup(word);
            return possivel_id;
        }
    }
    printf("Tabela cheia!\n");
    return -1;
}

void printHashValues(HashTable *h, FILE *stream){
    for(int i=0;i<h->size;i++){
        if(h->palavras[i]) fprintf(stream,"\t%s\t:\t%d\n", h->palavras[i], i);
    }
}
