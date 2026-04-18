#ifndef HASHING_H
#define HASHING_H

#define CONST_PRIME 3

typedef struct{
    char **palavras;
    int size;
} HashTable;

HashTable* initializeHashTable(int size);
int hashFunction(int size, char *palavra, int try_num);
int searchWord(HashTable *h, char *word);
int insertWord(HashTable *h, char *word);
void printHashValues(HashTable *h, FILE *stream);
void freeHashing(HashTable *h);

#endif