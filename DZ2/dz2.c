#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

Queue* pushToQueue(Queue* head, BinTree* ptr) {
	Queue* newNode = malloc(sizeof(Queue));

	newNode->info = ptr;
	newNode->next = head;
	head = newNode;

	return head;
}

BinTree* popFromQueue(Queue** head) {
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

void LevelOrder(BinTree* root) {
	Queue* order = NULL;
	BinTree* node = NULL;

	if (root != NULL) {
		order = pushToQueue(order, root);
		while ((node = popFromQueue(&order)) != NULL) {
			printf("%s\n", node->info);
			order = pushToQueue(order, node->left);
			order = pushToQueue(order, node->right);
		}
	}
}

int getMax(int a, int b) {
	return a > b ? a : b;
}

int getHeight(BinTree* root) {
	if (root == NULL) {
		return -1;
	}
	return 1 + getMax(getHeight(root->left), getHeight(root->right));
}

int main(void) {
	Queue* queue = NULL;
	BinTree* root = NULL;
	BinTree* current = NULL;
	BinTree* newNode = NULL;
	char info[256];

	scanf("%s", &info);
	while (strcmp(info, "-1") != 0) {
		newNode = malloc(sizeof(BinTree));
		newNode->left = NULL;
		newNode->right = NULL;
		strcpy(newNode->info, info);
		if (root == NULL) {
			root = newNode;
			queue = pushToQueue(queue, root);
		} //if
		else {
			current = peekQueue(queue);
			if (current->left == NULL) {
				current->left = newNode;
			}
			else {
				current->right = newNode;
				current = popFromQueue(&queue);
				queue = pushToQueue(queue, current->left);
				queue = pushToQueue(queue, current->right);
			}
		} //else
		scanf("%s", &info);
	} //while
	LevelOrder(root);
	printf("\n%d\n", getHeight(root));

	return 0;
}