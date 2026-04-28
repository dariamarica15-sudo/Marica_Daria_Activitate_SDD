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

// Functie pentru copierea primelor n elemente dintr-un vector de produse

struct Produs* copiazaPrimeleNElemente(struct Produs* vector, int nrElemente, int nrElementeCopiate) {

	if (nrElementeCopiate > nrElemente) {
		nrElementeCopiate = nrElemente;
	}

	struct Produs* vectorNou = (struct Produs*)malloc(sizeof(struct Produs) * nrElementeCopiate);

	// Copiem elementele din vectorul original in vectorul nou
	for (int i = 0; i < nrElementeCopiate; i++) {

		vectorNou[i].id = vector[i].id;

		vectorNou[i].denumire = (char*)malloc(strlen(vector[i].denumire) + 1);
		strcpy(vectorNou[i].denumire, vector[i].denumire);

		vectorNou[i].pret = vector[i].pret;
		vectorNou[i].categorie = vector[i].categorie;
	}

	return vectorNou;
}


// Functie pentru copierea produselor cu pret mai mare decat un prag dat
void copiazaProduseCuPretMaiMare(struct Produs* vector, int nrElemente, float prag,
	struct Produs** vectorNou, int* dimensiune) {

	*dimensiune = 0;

	// Parcurgem vectorul pentru a numara cate produse au pretul mai mare decat pragul
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].pret > prag) {
			(*dimensiune)++;
		}
	}

	// Alocam memorie pentru vectorul nou, avand dimensiunea egala cu numarul de produse care indeplinesc conditia
	*vectorNou = (struct Produs*)malloc(sizeof(struct Produs) * (*dimensiune));
	
	int k = 0;

	// Parcurgem din nou vectorul pentru a copia produsele care au pretul mai mare decat pragul in vectorul nou
	for (int i = 0; i < nrElemente; i++) {
		// Daca pretul produsului curent este mai mare decat pragul, il copiem in vectorul nou
		if (vector[i].pret > prag) {

			(*vectorNou)[k].id = vector[i].id;

			(*vectorNou)[k].denumire = (char*)malloc(strlen(vector[i].denumire) + 1);
			strcpy((*vectorNou)[k].denumire, vector[i].denumire);

			(*vectorNou)[k].pret = vector[i].pret;
			(*vectorNou)[k].categorie = vector[i].categorie;

			k++;
		}
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

	// Copiem primele 2 elemente din vectorul de produse
	struct Produs* primeleDoua = copiazaPrimeleNElemente(prod, nrProduse, 2);
	printf("Primele 2 produse:\n");
	afisareVector(primeleDoua, 2);
	
	// Copiem produsele cu pret mai mare decat 1800
	struct Produs* produseScumpe = NULL;
	int nrProduseScumpe = 0;

	copiazaProduseCuPretMaiMare(prod, nrProduse, 1800, &produseScumpe, &nrProduseScumpe);

	printf("Produse cu pret mai mare decat 1800:\n");
	afisareVector(produseScumpe, nrProduseScumpe);

	// Eliberam memoria alocata pentru denumiri in vectorul original
	for (int i = 0; i < nrProduse; i++) {
		free(prod[i].denumire);
	}

	// Eliberam memoria alocata pentru primele doua produse copiate
	for (int i = 0; i < 2; i++) {
		free(primeleDoua[i].denumire);
	}

	// Eliberam memoria alocata pentru produsele scumpe copiate
	for (int i = 0; i < nrProduseScumpe; i++) {
		free(produseScumpe[i].denumire);
	}

	free(produseScumpe);

	free(prod);
	return 0;
}