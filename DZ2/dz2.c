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
/* REKURZIVNA VISINA STABLA:

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

void playGame(BinTree* root) {
	BinTree* next = NULL;
	BinTree* sol = NULL;
	BinTree* newNode = NULL;
	char answer[4];
	char solAnswer[4];
	char solution[25];
	char question[255];

	next = root;
	while (1) {
		printf("%s\n", next->info);
		gets(answer);
		if (strcmp(answer, "yes") == 0) {
			if (next->left->left == NULL) {
				strcpy(solution, next->left->info);
				sol = next->left;
				break;
			}
			next = next->left;
		}
		if (strcmp(answer, "no") == 0) { //nepotrebno ako se pretpostavi da ce odgovori uvek biti yes ili no
			if (next->right->left == NULL) {
				strcpy(solution, next->right->info);
				sol = next->right;
				break;
			}
			next = next->right;
		}
	}
 	printf("%s\n", solution);
	printf("Da li je odgovor tacan?");
	gets(solAnswer);
	if (strcmp(solAnswer, "no") == 0) {
		printf("Unesite tacan odgovor: ");
		gets(solution);
		printf("Unesite pitanje kojim mogu da razlikujem pojam %s od pojma %s:\n", solution, sol->info);
		gets(question);
		newNode = malloc(sizeof(BinTree));
		newNode->left = NULL;
		newNode->right = NULL;
		strcpy(newNode->info, question);
		if (strcmp(answer, "yes") == 0) {
			next->left = newNode;
		}
		else {
			next->right = newNode;
		}
		next = newNode;
		newNode = malloc(sizeof(BinTree));
		newNode->left = NULL;
		newNode->right = NULL;
		strcpy(newNode->info, solution);
		next->left = newNode;
		next->right = sol;
	}
}

int main(void) {
	Queue* queue = NULL;
	BinTree* root = NULL;
	BinTree* current = NULL;
	BinTree* newNode = NULL;
	char info[256];
	int again;

	//scanf("%s", &info);
	gets(info);
	while (strcmp(info, "-1") != 0) {
		if (strcmp(info, "0") == 0) {
			gets(info);
			current = deleteFromQueue(&queue);
			continue;
		}
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
	} //while
	LevelOrder(root);
	//printf("\n%d\n", getHeight(root));
	printf("\nGAME\n");
	while (1) {
		playGame(root);
		scanf("%d", &again);
		getchar();
		if (again == 0) {
			break;
		}
	}
	return 0;
}