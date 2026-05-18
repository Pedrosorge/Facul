#ifndef SYMBOL_TABLES
#define SYMBOL_TABLES
#include "../data_structures/hashing.h"
#include "../data_structures/trie.h"
#include <stdlib.h>


// HashTables com os tokens de cada valor lido
extern HashTable *ID_reserved, *ID_variables, *ID_operators;

// Dicionário de palavras reservadas
extern Trie *t;

void initialize_hashtables();
void free_hashtables();
void initialize_reserved_words_trie();
void free_reserved_words_trie();


#endif