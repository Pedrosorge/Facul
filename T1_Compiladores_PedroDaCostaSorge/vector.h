#ifndef VECTOR_H
#define VECTOR_H

typedef struct VectorErrorNode{
    struct VectorErrorNode *next;
    int value_err;
    int value_line;
}VectorErrorNode;

typedef struct{
    VectorErrorNode *head;
    VectorErrorNode *tail;
} VectorError;

VectorError* initializeVector();
void push_back(VectorError *vec,int v, int line);

#endif