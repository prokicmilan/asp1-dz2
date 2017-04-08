#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//struktura binarnog stabla
typedef struct bintree {
	char info[256];
	struct bintree* left;
	struct bintree* right;
} BinTree;

//struktura reda
typedef struct queue {
	BinTree* info;
	struct queue* next;
} Queue;

Queue* insertQueue(Queue* head, BinTree* ptr) {
	Queue* newNode = malloc(sizeof(Queue));

	newNode->info = ptr;
	newNode->next = head;
	head = newNode;

	return head;
}

BinTree* deleteFromQueue(Queue** head) {
	Queue* curr = *head;
	Queue* prev = NULL;
	BinTree *old = NULL;
	
	prev = curr;
	while (curr->next != NULL) {
		prev = curr;
		curr = curr->next;
	}

	old = curr->info;
	if (curr == *head) {
		*head = NULL;
	}
	prev->next = NULL;
	free(curr);
	curr = NULL;

	return old;
}

BinTree* peekQueue(Queue* head) {
	Queue* curr = head;

	while (curr->next != NULL) {
		curr = curr->next;
	}

	return curr->info;
}

//ispis stabla - obrisati
void LevelOrder(BinTree* root) {
	Queue* order = NULL;
	BinTree* node = NULL;

	node = root;
	order = insertQueue(order, node);
	while (order != NULL) {
		node = deleteFromQueue(&order);
		printf("%s\n", node->info);
		if (node->left != NULL) {
			order = insertQueue(order, node->left);
		}
		if (node->right != NULL) {
			order = insertQueue(order, node->right);
		}
	}
}

int getHeight(BinTree *root) {
	Queue* order = NULL;
	BinTree* node = NULL;
	int numRead = 0;

	node = root;
	order = insertQueue(order, root);
	while (order != NULL) {
		node = deleteFromQueue(&order);
		numRead++;
		if (node->left != NULL) {
			order = insertQueue(order, node->left);
		}
		if (node->right != NULL) {
			order = insertQueue(order, node->right);
		}
	}
	return (int)(floor(log10(numRead)/log10(2)));
}
/* REKURZIVNO RESENJE:

int getMax(int a, int b) {
	return a > b ? a : b;
}

int getHeight(BinTree* root) {
	if (root == NULL) {
		return -1;
	}
	return 1 + getMax(getHeight(root->left), getHeight(root->right));
}
*/

int main(void) {
	Queue* queue = NULL;
	BinTree* root = NULL;
	BinTree* current = NULL;
	BinTree* newNode = NULL;
	char info[256];

	//scanf("%s", &info);
	gets(info);
	printf("%s\n", info);
	while (strcmp(info, "-1") != 0) {
		newNode = malloc(sizeof(BinTree));
		newNode->left = NULL;
		newNode->right = NULL;
		strcpy(newNode->info, info);
		if (root == NULL) {
			root = newNode;
			queue = insertQueue(queue, root);
		} //if
		else {
			current = peekQueue(queue);
			if (current->left == NULL) {
				current->left = newNode;
			}
			else {
				current->right = newNode;
				current = deleteFromQueue(&queue);
				queue = insertQueue(queue, current->left);
				queue = insertQueue(queue, current->right);
			}
		} //else
		gets(info);
		printf("%s\n", info);
	} //while
	LevelOrder(root);
	printf("\n%d\n", getHeight(root));

	return 0;
}