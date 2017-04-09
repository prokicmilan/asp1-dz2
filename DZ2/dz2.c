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

BinTree* formTree() {
	BinTree* root = NULL;
	BinTree* current = NULL;
	BinTree* newNode = NULL;
	Queue* queue = NULL;
	char info[255];

	printf("\n\"NULL\" za nepostojeci cvor, \"end\" za kraj unosa\n");
	gets(info);
	while (strcmp(info, "end") != 0) {
		if ((strcmp(info, "null") == 0 || strcmp(info, "NULL") == 0) && queue != NULL) {
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

	return root;
}

BinTree* deleteBase(BinTree* root) {
	Queue* order = NULL;
	BinTree* node = NULL;
	BinTree* old = NULL;
	if (root == NULL) {
		return NULL;
	}
	else {
		node = root;
		order = insertQueue(order, node);
		while (order != NULL) {
			node = deleteFromQueue(&order);
			if (node->left != NULL) {
				order = insertQueue(order, node->left);
			}
			if (node->right != NULL) {
				order = insertQueue(order, node->right);
			}
			free(node);
		}
	}
	return NULL;
}

void printMenu() {
	printf("1. Formiranje baze znanja.\n");
	printf("2. Brisanje baze znanja\n");
	printf("3. Igra\n");
	printf("4. Visina stabla baze znanja\n");
	printf("0. Kraj\n");
}


//TODO: ako je baza prazna, odmah predji na unos pojma
void playGame(BinTree** root) {
	BinTree* next = NULL;
	BinTree* sol = NULL;
	BinTree* newNode = NULL;
	char answer[4];
	char solAnswer[4];
	char solution[25];
	char question[255];

	next = *root;
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
		if (strcmp(answer, "no") == 0) { //nepotrebno ako se pretpostavi da ce odgovori uvek biti yes ili no, pruza zastitu od nepostojecih odgovora
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
		printf("Unesite odgovor: ");
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

//ne radi kako treba
int getHeight(BinTree *root) {
	Queue* order = NULL;
	BinTree* node = NULL;
	int numRead = 0;

	if (root == NULL) {
		return 0;
	}
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

int main(void) {
	BinTree* knowledgeBase = NULL;
	int menuChoice;
	int cont = 1;
	int baseChoice;
	
	while (cont) {
		printMenu();
		scanf("%d", &menuChoice);
		getchar();
		switch (menuChoice) {
			case 1:
				knowledgeBase = formTree();
				break;
			case 2:
				knowledgeBase = deleteBase(knowledgeBase);
				break;
			case 3:
				printf("Moguci odgovori su \"yes\" i \"no\"\n");
				playGame(&knowledgeBase);
				break;
			case 4:
				printf("Visina baze je %d\n", getHeight(knowledgeBase));
				break;
			case 0:
				cont = 0;
				break;
			default:
				printf("Nepostojeca opcija.\n");
				break;
		}
	}
	return 0;
}