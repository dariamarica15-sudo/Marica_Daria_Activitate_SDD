#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Produs {
	int id;
	char* denumire;
	float pret;
	char categorie;
};

// Functie pentru initializarea unui produs
struct Produs initializare(int id, const char* denumire, float pret, char categorie) {
	struct Produs p;

	p.id = id;
	p.denumire = (char*)malloc(strlen(denumire) + 1);
	strcpy(p.denumire, denumire);
	p.pret = pret;
	p.categorie = categorie;

	return p;
}

// Functie pentru afisarea unui produs
void afisare(struct Produs p) {
	printf("Id: %d\n", p.id);
	printf("Denumire: %s\n", p.denumire);
	printf("Pret: %.2f\n", p.pret);
	printf("Categorie: %c\n\n", p.categorie);
}

// Functie pentru eliberarea memoriei alocate pentru un produs
int main() {
	struct Produs p1 = initializare(1, "Laptop", 3500.5, 'A');

	afisare(p1);

	free(p1.denumire);

	return 0;
}