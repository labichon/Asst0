#include <stdio.h>
#include <stdlib.h>

//declared typedef for node
typedef struct node{
	int key;
	int value;
	struct node *next;
} node;

//not sure if this needed, but just in case
int mod (int x){
	x = abs(x%10000);
	return x;
}

//initialize the HashMap
node** fill(struct node** HashMap){
	for(int i = 0; i<10000; i++){
		HashMap[i] = NULL;
	}
}

//These should be all the functions needed for HashMap node stuff for now
/*
void nullinsert(struct node** HashMap, int x, int i){
	struct node* newnode=(struct node*) malloc(sizeof(struct node));
        newnode->value = x;
        newnode->next = NULL;
        HashMap[i] = newnode;
}

void insert(struct node** HashMap, int x){
	int i = mod(x);
	if(HashMap[i] == NULL){
		nullinsert(HashMap, x, i);
	}else{
		struct node* temp = HashMap[i];
		while(temp->next != NULL){
			temp = temp->next;
		}
		struct node* newnode=(struct node*) malloc(sizeof(struct node));
		newnode->value = x;
		newnode->next = NULL;
		temp->next = newnode;
	}
}

int search(struct node** HashMap, int x){
	int i = mod(x);
	if(HashMap[i] == NULL){
		return 0;
	}else{
		struct node* temp = HashMap[i];
		while(temp != NULL){
			if(temp->value == x){	
				return 1;
			}
			temp = temp->next;
		}
 	}
 	return 0;
}
*/

int main(int argc, char *argv[]){

	if(argc<2){
		printf("aids\n");
	}

	char *aidsstring = argv[1];	
	//printf("%s\n", aidsstring);
	
	//int value = atoi(aidsstring);
	//printf("%d\n", value);

	struct node* HashMap[10000];
	fill(HashMap);



	return 1;
}


