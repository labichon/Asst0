#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tokenizer.h"

//declared typedef for node
typedef struct node{
	char *key;
	char *value;
	struct node *next;
} node;

int main(int argc, char *argv[]){

	// Input only have 1 arg
	if(argc !=  2){
		printf("aids\n");
		return 1;
	}

	char *aidsstring = argv[1];	
	//printf("%s\n", aidsstring);
	
	//int value = atoi(aidsstring);
	//printf("%d\n", value);
	
	int hashLen = 10000;

	// Allocate space for and initialize hashmap
	node **HashMap = (node**) malloc(sizeof(node*) * hashLen);
	initHash(HashMap, hashLen);
	
	// Insert Special Characters
	insert(HashMap, "(", "left parenthesis");
	insert(HashMap, ")", "right parenthesis");
	insert(HashMap, "[", "left bracket");
	insert(HashMap, "]", "right bracket");
	insert(HashMap, ".", "structure member");
	insert(HashMap, "->", "structure pointer");
	insert(HashMap, "sizeof", "sizeof");
	insert(HashMap, ",", "comma");
	insert(HashMap, "!", "negate");
	insert(HashMap, "~", "1s complement");
	insert(HashMap, ">>", "shift right");
	insert(HashMap, "<<", "shift left");
	insert(HashMap, "^", "bitwise XOR");
	insert(HashMap, "|", "bitwise OR");
	insert(HashMap, "++", "increment");
	insert(HashMap, "--", "decrement");
	insert(HashMap, "+", "addition");
	insert(HashMap, "/", "division");
	insert(HashMap, "||", "logical OR");
	insert(HashMap, "&&", "logical AND");
	insert(HashMap, "?","conditional true");
	insert(HashMap, ":", "conditional false");
	insert(HashMap, "==", "equality test");
	insert(HashMap, "!=", "inequality test");
	insert(HashMap, "<", "less than test");
	insert(HashMap, ">", "greater than test");
	insert(HashMap, "<=", "less than or equal test");
	insert(HashMap, ">=", "greater than or equal test");
	insert(HashMap, "=", "assignment");
	insert(HashMap, "+=", "plus equals");
	insert(HashMap, "-=", "minus equals");
	insert(HashMap, "*=", "times equals");
	insert(HashMap, "/=", "divide equals");
	insert(HashMap, "%=", "mod equals");
	insert(HashMap, ">>=", "shift right equals");
	insert(HashMap, "<<=", "shift left equals");
	insert(HashMap, "&=", "bitwise AND equals");
	insert(HashMap, "^=", "bitwise XOR equals");
	insert(HashMap, "|=", "bitwise OR equals");
	insert(HashMap, "&", "AND/address operator");
	insert(HashMap, "-", "minus/subtract operator");
	insert(HashMap, "*", "multiply/dereference operator");


	// Possibilities: Word, Hex, Dec, Oct, FP
	// Precedence: Space > Special  (except '-' and '.' > Word
	// > Hex > FP > Dec  > Oct
	// Exceptions: Dec + '.' + Dec = Float
	// Dec + '.' + Dec + 'e-' + Dec = Float
	// 

	return 0;
}

//function to hash our values
int hash(char* key, size_t keyLen){
	int x = 0;
	for(int i = 0; i < keyLen; i++) x += key[i];
	x = abs(x%10000);
	return x;
}


//initialize HashMap
void initHash(node **HashMap, int hashLen){
	for(int i = 0; i<hashLen; i++){
		HashMap[i] = NULL;
	}
}

/* Hashmap insert
 * Input: HashMap (node*), key (str), value to insert (str)
 * Output: N/A
 * Note: We do not account for duplicates
*/
void insert(node **HashMap, char *key, char *value){
	int i = hash(key, strlen(key));
	node *newNode = (node*) malloc(sizeof(node));
	newNode->key = key;
	newNode->value = value;

	newNode->next = HashMap[i];
	HashMap[i] = newNode;
}

/* HashMap Search
 * Input: Hashmap (node*) , Hashmap key (str), size of key (size_t)
 * Output: NULL if not found, node found otherwise
*/
node* search(node **HashMap, char *key, size_t size){
	int i = hash(key, size);
	if(HashMap[i] != NULL) {
		node *temp = HashMap[i];
		while(temp != NULL){
			if(temp->key == key){	
				return temp;
			}
			temp = temp->next;
		}
 	}
 	return NULL;
}

