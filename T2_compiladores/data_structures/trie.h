#ifndef TRIE_H
#define TRIE_H

typedef struct Node{
    int end;
    struct Node*next[26];
}Node;

typedef struct{
    Node *root;
} Trie;

void initializeTrie(Trie *t);
void insertToTrie(Trie *t, char *s);
int searchTerm(Trie *t, char *s);
void freeTrie(Trie *t);
void freeTrieNode(Node *n);

#endif