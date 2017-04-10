//TODO: Suma stabala (po jedno za svaku kategoriju)

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
	
	if (*head != NULL) {
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
	}

	return old;
}

BinTree* peekQueue(Queue* head) {
	Queue* curr = head;

	while (head != NULL && curr->next != NULL) {
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
	while (root != NULL && order != NULL) {
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

	printf("\n\"null\" ako je otac list, \"skip\" ako cvor ne postoji, \"end\" za kraj unosa\n");
	gets(info);
	while (strcmp(info, "end") != 0) {
		if (strcmp(info, "null") == 0 && queue != NULL) {
			gets(info);
			current = deleteFromQueue(&queue);
			continue;
		}
		newNode = malloc(sizeof(BinTree));
		newNode->left = NULL;
		newNode->right = NULL;
		strcpy(newNode->info, info);
		if (root == NULL && strcmp(info, "skip") != 0) {
			root = newNode;
			queue = insertQueue(queue, root);
		} //if
		else {
			current = peekQueue(queue);
			if (current->left == NULL) {
				if (strcmp(info, "skip") != 0) {
					current->left = newNode;
				}
				else {
					free(newNode);
					gets(info);
					newNode = malloc(sizeof(BinTree));
					newNode->left = NULL;
					newNode->right = NULL;
					strcpy(newNode->info, info);
					current->right = newNode;
					current = deleteFromQueue(&queue);
					queue = insertQueue(queue, current->right);
				}
			}
			else if (strcmp(info, "skip") != 0) {
					current->right = newNode;
					current = deleteFromQueue(&queue);
					queue = insertQueue(queue, current->left);
					queue = insertQueue(queue, current->right);
				}
			else {
				free(newNode);
				current = deleteFromQueue(&queue);
				queue = insertQueue(queue, current->left);
			}
		}
		gets(info);
	} //while

	return root;
}

void deleteBase(BinTree** root) {
	Queue* order = NULL;
	BinTree* node = NULL;
	BinTree* old = NULL;
	if (*root != NULL) {
		node = *root;
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
	*root = NULL;
}

void printMenu() {
	printf("1. Formiranje baze znanja.\n");
	printf("2. Brisanje baze znanja\n");
	printf("3. Igra\n");
	printf("4. Visina stabla baze znanja\n");
	printf("0. Kraj\n");
}


void playGame(BinTree** root) {
	BinTree* next = NULL;
	BinTree* sol = NULL;
	BinTree* newNode = NULL;
	char answer[4];
	char solAnswer[4];
	char solution[25] = "Nepoznata kategorija\n";
	char question[255];

	if (*root == NULL) {
		printf("Baza znanja je prazna\n");
		printf("Unesite koreno pitanje:\n");
		gets(question);
		newNode = malloc(sizeof(BinTree));
		newNode->left = NULL;
		newNode->right = NULL;
		strcpy(newNode->info, question);
		*root = newNode;
		return;
	}
	printf("Moguci odgovori su \"yes\" i \"no\"\n");
	next = *root;
	while (next != NULL) {
		printf("%s\n", next->info);
		gets(answer);
		if (strcmp(answer, "yes") == 0) {
			if (next->left != NULL && next->left->left == NULL && next->left->right == NULL) {
				strcpy(solution, next->left->info);
				sol = next->left;
				break;
			}
			next = next->left;
		}
		else if (strcmp(answer, "no") == 0) {
			if (next->right != NULL && next->right->right == NULL && next->right->left == NULL) {
				strcpy(solution, next->right->info);
				sol = next->right;
				break;
			}
			next = next->right;
		}
	}
	if (strcmp(solution, "Nepoznata kategorija\n") != 0) {
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
	else {
		printf("%s", solution);
	}
}

int getHeight(BinTree *root) {
	Queue* order = NULL;
	BinTree* node = NULL;
	int height = 0;
	int toPopOld = 0;
	int toPopNew = 0;

	if (root == NULL) {
		return 0;
	}
	order = insertQueue(order, root);
	toPopNew = 1;
	while (1) {
		if (order == NULL) {
			return --height;
		}
		height++;
		toPopOld = toPopNew;
		toPopNew = 0;
		while (toPopOld > 0) {
			node = deleteFromQueue(&order);
			if (node->left != NULL) {
				order = insertQueue(order, node->left);
				toPopNew++;
			}
			if (node->right != NULL) {
				order = insertQueue(order, node->right);
				toPopNew++;
			}
			toPopOld--;
		}
	}
	return 0;
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
	
	while (cont) {
		printMenu();
		scanf("%d", &menuChoice);
		getchar();
		switch (menuChoice) {
			case 1:
				deleteBase(&knowledgeBase);
				knowledgeBase = formTree();
				LevelOrder(knowledgeBase);
				break;
			case 2:
				deleteBase(&knowledgeBase);
				break;
			case 3:
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