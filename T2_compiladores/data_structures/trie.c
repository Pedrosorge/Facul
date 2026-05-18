#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "trie.h"

// Inicializa a Trie
void initializeTrie(Trie *t){
    t->root = (Node *)calloc(1,sizeof(Node));
}

// Insere palavra na Trie
void insertToTrie(Trie *t,char *s){
    
    Node *curr_node = t->root;
    
    for(int i=0;i<strlen(s);i++){
        
        int letter_position = tolower(s[i]) - 'a';
        
        if(curr_node->next[letter_position]==NULL) curr_node->next[letter_position] = (Node *)calloc(1,sizeof(Node));
        
        curr_node = curr_node->next[letter_position];
    }
    curr_node->end = 1;

}

// Verifica de uma palavra está na Trie
int searchTerm(Trie *t, char *s){

    Node *curr_node = t->root;

    for(int i=0;i<strlen(s);i++){
        
        int letter_position = tolower(s[i]) - 'a';

        if(curr_node->next[letter_position] == NULL) return 0;

        curr_node= curr_node->next[letter_position];

    }

    return curr_node->end;

}

// Desaloca os nós da Trie
void freeTrieNode(Node *n){
    for(int i=0;i<26;i++){
        if(n->next[i]!=NULL) freeTrieNode(n->next[i]);
    }
    free(n);
}

// Desaloca Trie
void freeTrie(Trie *t){
    Node *aux = t->root;
    for(int i=0;i<26;i++){
        if(aux->next[i]!=NULL) freeTrieNode(aux->next[i]);
    }
    free(t->root);
    free(t);
}

















