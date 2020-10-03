// Header file for tokenizer.c
#ifndef TOKEN_H
#define TOKEN_H

typedef struct node node;
int hash(char*, size_t);
void initHash(node**, int);
void insert(node**, char*, char*);
node* search(node**, char*, size_t);

#endif
