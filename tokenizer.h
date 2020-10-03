// Header file for tokenizer.c
#ifndef TOKEN_H
#define TOKEN_H

typedef struct node node;
typedef enum {
	None,
	Special,
	Word,
	Exp,
	Float,
	Dec,
	Hex,
	Oct
} types;
int hash(char*, size_t);
void initHash(node**, int);
void insert(node**, char*, char*);
node* search(node**, char*, size_t);
char* getTypeStr(types);

#endif
