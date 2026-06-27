#ifndef VECTOR_H
#define VECTOR_H

typedef struct VectorErrorNode{
    struct VectorErrorNode *next;
    char *details;
    int value_err;
    int value_line;
}VectorErrorNode;

typedef struct{
    VectorErrorNode *head;
    VectorErrorNode *tail;
} VectorError;

VectorError* initializeVector();
void push_back(VectorError *vec, char *details,int v, int line);
void freeVector(VectorError *v);

#endif