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

// Functie pentru afisarea unui vector de produse
void afisareVector(struct Produs* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisare(vector[i]);
	}
}

// Functie pentru eliberarea memoriei alocate pentru un produs
int main() {
	int nrProduse = 3;

	struct Produs* prod = (struct Produs*)malloc(sizeof(struct Produs) * nrProduse);

	prod[0] = initializare(1, "Laptop", 3500.00, 'B');
	prod[1] = initializare(2, "Telefon", 1500.00, 'T');
	prod[2] = initializare(3, "Tableta", 2000.00, 'S');

	afisareVector(prod, nrProduse);

	for (int i = 0; i < nrProduse; i++) {
		free(prod[i].denumire);
	}

	free(prod);
	return 0;
}