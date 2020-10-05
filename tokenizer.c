#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tokenizer.h"

#define HASH_LEN 10000

//declared typedef for node
typedef struct node{
	char *key;
	char *value;
	struct node *next;
} node;

int main(int argc, char *argv[]){

	// Input only have 1 arg
	if(argc !=  2){
		printf("error\n");
		return 1;
	}

	char *input = argv[1];
	
	int hashLen = HASH_LEN;
	
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

	// Insert keywords
	char *kws[] = {"auto", "double", "int", "struct", "break", "else", "long", 
                     "switch", "case", "enum", "register", "typedef", "char", 
		     "extern", "return", "union", "continue", "for", "signed", 
		     "void", "do", "if", "static", "while", "default", "goto", 
		     "sizeof", "volatile", "const", "float", "short", "unsigned"};
	for (int i = 0; i < 32; i++) {
		insert(HashMap, kws[i], "keyword");
	}

	// Start list of tokens
	node *head = (node*) malloc(sizeof(node));
	head -> key = NULL;
	head -> value = NULL;
	head -> next = NULL;
	node *curr = head;

	int last = 0;

	types currType = None;
	
	// Loop until end of input string
	int i = 0;
	while (input[i] != '\0') {
		
		//printf("Input: %c, Type: %d\n", input[i], currType);
		int next = 0;

		// Find whether full string is a special character
		node *special = search(HashMap, &input[last], i - last + 1);
		// Find whether we have a special char
		node *specChar = search(HashMap, &input[i], 1);

		// Find whether or not double or single quotes are in the string
		if (currType == DoubleQuotes) {
			if (input[i] == '\"') next = 1;
		} else if (currType == SingleQuotes) {
			if (input[i] == '\'') next = 1;
		} else if (input[i] == '\"') {
			currType = DoubleQuotes;
		} else if (input[i] == '\'') {
			currType = SingleQuotes;
		} else if (isspace(input[i])) {
			// If current character is whitespace
			if (currType != None) {
				next = 1;
			} else {
				last = i+1;
			}
		} else if (special != NULL) {
			// Found special string sequence
			
			// If a keyword is followed by alphanum char, treat as a word
			if (strcmp(special -> value, "keyword") == 0 &&
                            isalnum(input[i+1])) {
				currType = Word;

			} else {
				curr -> value = special -> value;
				currType = Special;
			}
		} else if (currType == None) {
			// Current type not yet known (new token)
			// Cases: Special Char, '0', Decimal, Word
			if (specChar != NULL) {
				// Case Special char
				curr -> value = specChar -> value;
				currType = Special;
			} else if (input[i] == '0') { 
				// Case '0': Possible Oct/Hex/Dec
				if ((input[i+1] == 'x'
                                           || input[i+1] =='X') && isxdigit(input[i+2])) {
					// Hexadecimal
					currType = Hex;
					i+=2;
					continue;
				} else {
					// Octal
					currType = Oct;
				}
			} else if (isdigit(input[i])) {
				// Case Decimal
				currType = Dec;
			} else if (isalpha(input[i])) {
				// Case Word
				currType = Word;
			} else {
				printf("Character %c not recognized", input[i]);
				return 1;
			}
		} else if (currType == Special) {
			// Word isn't a special token, so go to next
			next = 1;
		} else if (currType == Word) {
			if (!isalnum(input[i])) next = 1;
		}else if (currType == Dec) {
			// Cases: 1) '.' 2) Decimal 3) Non-decimal
			if (input[i] == '.' && isdigit(input[i+1])) {
				currType = Float;
				i++;
			} else {
				if (!isdigit(input[i])) next = 1;
			}

		} else if (currType == Float) {
			// 3 cases: 1) 'e' or 'e-' 2) decimal char 3) word or special
			if (input[i] == 'e' && (isdigit(input[i+1]))) {
				currType = Exp;
				i++;
			} else if (input[i] == 'e' && (input[i+1] == '-' || input[i+1] == '+')
                                   && isdigit(input[i+2])) {
				currType = Exp;
				i += 2;
			} else {
				if (!isdigit(input[i])) next = 1;
			}
		} else if (currType == Exp) {
			if (!isdigit(input[i])) next = 1;
		} else if (currType == Oct) {
			if (input[i] > '7' || input[i] < '0') {
				if (isdigit(input[i])) currType = Dec;
				else if (input[i] == '.' && isdigit(input[i+1])) {
					currType = Float;
					i++;
				}
				else next = 1;
			}
		} else if (currType == Hex) {
			if (!isxdigit(input[i])) next = 1;
		}

		if (next || input[i+1] == '\0') {
			// Copy relevant info to current node and create a new one
			if (input[i+1] == '\0') {
				i++;
			}	

			// Copy chars from last to i into node
			curr -> key = malloc(i - last + 1);
			memcpy(curr -> key, &input[last], i - last);
			(curr -> key)[i-last] = '\0';

			if (currType != Special) curr -> value = getTypeStr(currType);

			if (input[i] != '\0') {
				// Not last node, create next
				node *newNode = (node*) malloc(sizeof(node));
				newNode -> key = NULL;
				newNode -> value = NULL;
				newNode -> next = NULL;
				curr -> next = newNode;
				curr = curr -> next;

				// Copy type into next node
				currType = None;
				last = i;
			}
		} else {
			// Only increment i if we did not create a new node
			i++;
		}
	}
	printLL(head);
	
	// Free memory
	freeLL(head, 1);
	for (int i = 0; i < hashLen; i++) {
		freeLL(HashMap[i], 0);
	}
	free(HashMap);
	return 0;
}

//function to hash our values
int hash(char* key, size_t keyLen){
	int x = 0;
	for(int i = 0; i < keyLen; i++) x += key[i];
	x = abs(x % HASH_LEN);
	return x;
}


//initialize HashMap
void initHash(node **HashMap, int hashLen){
	for(int i = 0; i < hashLen; i++){
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
			if(strlen(temp->key) == size && memcmp(temp->key, key, size) == 0){	
				return temp;
			}
			temp = temp->next;
		}
 	}
 	return NULL;
}

/* Get Type From String
 * Input: type (types)
 * Output: String description of type
*/
char* getTypeStr(types type) {
	switch(type) {
		case Word:
			return "word";
		case Float:
			return "floating point";
		case Exp:
			return "floating point";
		case Dec:
			return "decimal";
		case Hex:
			return "hexadecimal integer";
		case Oct:
			return "octal integer";
		case SingleQuotes:
			return "string";
		case DoubleQuotes:
			return "string";
		default:
			return NULL;
	}
}

/* Print a Linked List
 * Input: Head of LL
 * Output: n/a
*/
void printLL(node *head) {
	for (node *curr = head; curr != NULL; curr = curr -> next) {
		printf("%s: \"%s\"\n", curr -> value, curr -> key);
	}
}

/* Free the nodes of a LL
 * Input: LL head, int (1 if freeing string, 0 otherwise)
 * Output: n/a
*/
void freeLL(node *head, int hasStr) {
	while (head != NULL) {
		if (hasStr) free(head -> key);
		node* toFree = head;
		head = head -> next;
		free(toFree);
	}

}
