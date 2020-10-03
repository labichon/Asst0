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

void printLL(node* head);

int main(int argc, char *argv[]){

	// Input only have 1 arg
	if(argc !=  2){
		printf("aids\n");
		return 1;
	}

	char *input = argv[1];	
	//printf("%s\n", aidsstring);
	
	//int value = atoi(aidsstring);
	//printf("%d\n", value);
	
	int hashLen = 10000;
	
	printf("hi");

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
		node *specChar = search(HashMap, &input[i], 1);

		if (isspace(input[i])) {
			// If current character is whitespace
			if (currType != None) {
				// TODO:Set current node value and create a new node
				next = 1;
				i++;
			} else {
				last = i+1;
			}
		} else if (special != NULL) {
			// Found special string sequence
			curr -> value = special -> value;
			currType = Special;
		} else if (currType == None) {
			// Current type not yet known (no chars)
			// Cases: Special Char, '0', Decimal, Word
			if (specChar != NULL) {
				// Case Special char
				curr -> value = specChar -> value;
				currType = Special;
			} else if (input[i] == '0') { 
				// Case '0': Possible Oct/Hex/Dec
				if (input[i+1] >= '0' && input[i+1] < '8') {
					// Octal
					currType = Oct;
					i++;
					continue;
				} else if ((input[i+1] == 'x'
                                           || input[i+1] =='X') && isxdigit(input[i+2])) {
					// Hexadecimal
					currType = Hex;
					i+=2;
					continue;
				} else {
					// Decimal
					currType = Dec;
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
			next = 1;
		} else if (currType == Word) {
			if (!isalnum(input[i])) next = 1;
		}	
		else if (currType == Dec) {
			// Cases: 1) '.' 2) Word or Special 3) Decimal
			if (input[i] == '.' && isdigit(input[i+1])) {
				currType = Float;
				i++;
			} else {
				if (!isdigit(input[i])) next = 1;
			}

		} else if (currType == Float) {
			// 3 cases: 1) 'e' or 'e-' 2) decimal char 3) word or special
			if (input[i] == 'e' && (isdigit(input[i+1]))) {
				// 1
				currType = Exp;
				i++;
			} else if (input[i] == 'e' && (input[i+1] == '-' || input[i+1] == '+')
                                   && isdigit(input[i+2])) {
				// 1
				currType = Exp;
				i += 2;
			} else {
				if (!isdigit(input[i])) next = 1;
			}
		} else if (currType == Exp) {
			if (!isdigit(input[i])) next = 1;
		} else if (currType == Oct) {
			if (input[i] > '7' || input[i] < '0') next = 1;
		} else if (currType == Hex) {
			if (!isxdigit(input[i])) next = 1;
		}

		if (next) {
			// Copy relevant info to current node and create a new one
			
			// Copy chars from last to i into node
			curr -> key = malloc(i - last + 1);
			memcpy(curr -> key, &input[last], i - last);
			(curr -> key)[i-last] = '\0';

			if (currType != Special) curr -> value = getTypeStr(currType);

			node *newNode = (node*) malloc(sizeof(node));
			newNode -> key = NULL;
			newNode -> value = NULL;
			newNode -> next = NULL;
			curr -> next = newNode;
			curr = curr -> next;

			// Copy type into next node
			currType = None;
			last = i;

		} else if (input[i+1] == '\0') {
			curr -> key = malloc(++i - last + 1);
			memcpy(curr -> key, &input[last], i - last);
			(curr -> key)[i-last] = '\0';
			if (currType != Special) curr -> value = getTypeStr(currType);

		} else {
			i++;
		}
	}
	printLL(head);

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
			if(strlen(temp->key) == size && memcmp(temp->key, key, size) == 0){	
				return temp;
			}
			temp = temp->next;
		}
 	}
 	return NULL;
}

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
		default:
			return NULL;
	}
}

void printLL(node* head) {
	for (node *curr = head; curr != NULL; curr = curr -> next) {
		printf("%s: %s\n", curr -> value, curr -> key);
	}
}
