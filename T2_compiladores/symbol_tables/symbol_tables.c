#include "symbol_tables.h"


// HashTables com os tokens de cada valor lido
HashTable *ID_reserved, *ID_variables, *ID_operators;

// Inicializa as HashTables
void initialize_hashtables(){
    ID_reserved = initializeHashTable(100003);
    ID_variables = initializeHashTable(100003);
    ID_operators = initializeHashTable(97);
}

void free_hashtables(){
    freeHashing(ID_operators);
    freeHashing(ID_reserved);
    freeHashing(ID_variables);
}

// Dicionário de palavras reservadas
Trie *t;

// Inicializa Trie
void initialize_reserved_words_trie(){
    t = (Trie *)malloc(sizeof(Trie));
    initializeTrie(t); 
    
    // Se adicionar ou remover palavras reservadas lembrar de aumentar ou reduzir a variável 'size'
    char *reservadas[] = {"program", "var", "integer" , "procedure", "begin", "if", "then", "else", "end", "read", "write", "while", "do", "label", "function", "or", "and", "div"};
    int size = 18;
    
    for(int i=0;i<size;i++){
        insertToTrie(t,reservadas[i]);
    }
}

void free_reserved_words_trie(){
    freeTrie(t);
}