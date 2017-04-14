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

BinTree* formTree(BinTree* root) {
	BinTree* current = NULL;
	BinTree* newNode = NULL;
	BinTree* defaultNode = NULL;
	Queue* queue = NULL;
	char info[255];
	int catChoice;

	deleteBase(&root);
	printf("\n\"null\" ako je otac list, \"end\" za kraj unosa\n");
	gets(info);
	while (strcmp(info, "end") != 0) {
		if (strcmp(info, "null") == 0 && queue != NULL) {
			gets(info);
			current = deleteFromQueue(&queue);
			current->left = current->right = NULL;
			continue;
		}
		newNode = malloc(sizeof(BinTree));
		newNode->left = newNode->right = NULL;
		strcpy(newNode->info, info);
		if (root == NULL) {
			root = newNode;
			queue = insertQueue(queue, root);
		}
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
		}
		gets(info);
	} //while

	return root;
}

void printMenu() {
	printf("1. Formiranje baze znanja.\n");
	printf("2. Brisanje baze znanja\n");
	printf("3. Igra\n");
	printf("4. Visina stabla baze znanja\n");
	printf("0. Kraj\n");
}

void playGame(BinTree** root, char* category) {
	BinTree* next = NULL;
	BinTree* parent = NULL;
	BinTree* sol = NULL;
	BinTree* newNode = NULL;
	char answer[4];
	char solAnswer[4];
	char solution[25];
	char question[255];

	if (*root == NULL) {
		printf("Baza znanja je prazna\n");
		printf("Unesite zamisljen pojam:\n");
		gets(solution);
		printf("Unesite pitanje koje vodi do pojma:\n");
		gets(question);
		newNode = malloc(sizeof(BinTree));
		newNode->left = NULL;
		newNode->right = NULL;
		strcpy(newNode->info, question);
		*root = newNode;
		newNode = malloc(sizeof(BinTree));
		newNode->left = NULL;
		newNode->right = NULL;
		strcpy(newNode->info, category);
		(*root)->right = newNode;
		newNode = malloc(sizeof(BinTree));
		newNode->left = NULL;
		newNode->right = NULL;
		strcpy(newNode->info, solution);
		(*root)->left = newNode;
		return;
	}
	printf("Moguci odgovori su \"yes\" i \"no\"\n");
	next = *root;
	while (next != NULL) {
		printf("%s\n", next->info);
		gets(answer);
		parent = next;
		if (strcmp(answer, "yes") == 0) {
			if (next->left != NULL) {
				next = next->left;
			}
			if (next->left == NULL && next->right == NULL) {
				strcpy(solution, next->info);
				sol = next;
				next = NULL;
			}
		}
		else if (strcmp(answer, "no") == 0) {
			if (next->right != NULL) {
				next = next->right;
			}
			if (next->left == NULL && next->right == NULL) {
				strcpy(solution, next->info);
				sol = next;
				next = NULL;
			}
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
			parent->left = newNode;
		}
		else {
			parent->right = newNode;
		}
		parent = newNode;
		newNode = malloc(sizeof(BinTree));
		newNode->left = NULL;
		newNode->right = NULL;
		strcpy(newNode->info, solution);
		parent->left = newNode;
		parent->right = sol;
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
	BinTree* zivotinje = NULL;
	BinTree* predmeti = NULL;
	BinTree* biljke = NULL;
	BinTree* knowledgeBase[3] = { NULL, NULL, NULL };
	int menuChoice;
	int catChoice;
	int cont = 1;
	char* categories[3] = { "zivotinja", "predmet", "biljka" };
	
	while (cont) {
		printMenu();
		scanf("%d", &menuChoice);
		getchar();
		switch (menuChoice) {
			case 1:
				printf("Kategorije su: \n1. Zivotinje\n2. Predmeti\n3. Biljke\nOdaberite zeljenu kategoriju: ");
				scanf("%d", &catChoice);
				getchar();
				if (catChoice < 0 || catChoice >= 4) {
					printf("Nepostojeca kategorija\n");
					break;
				}
				knowledgeBase[catChoice - 1] = formTree(knowledgeBase[catChoice - 1]);
				LevelOrder(knowledgeBase[catChoice - 1]);
				break;
			case 2:
				printf("Kategorije su: \n1. Zivotinje\n2. Predmeti\n3. Biljke\nOdaberite zeljenu kategoriju: ");
				scanf("%d", &catChoice);
				if (catChoice < 0 || catChoice >= 4) {
					printf("Nepostojeca kategorija\n");
					break;
				}
				deleteBase(&knowledgeBase[catChoice - 1]);
				break;
			case 3:
				printf("Kategorije su: \n1. Zivotinje\n2. Predmeti\n3. Biljke\nOdaberite zeljenu kategoriju: ");
				scanf("%d", &catChoice);
				getchar();
				if (catChoice < 0 || catChoice >= 4) {
					printf("Nepostojeca kategorija\n");
					break;
				}
				playGame(&knowledgeBase[catChoice - 1], categories[catChoice - 1]);
				break;
			case 4:
				printf("Kategorije su: \n1. Zivotinje\n2. Predmeti\n3. Biljke\nOdaberite zeljenu kategoriju:\n");
				scanf("%d", &catChoice);
				if (catChoice < 0 || catChoice >= 4) {
					printf("Nepostojeca kategorija\n");
					break;
				}
				printf("Visina baze %s je %d\n",categories[catChoice - 1], getHeight(knowledgeBase[catChoice - 1]));
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