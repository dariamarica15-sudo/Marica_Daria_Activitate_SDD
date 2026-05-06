#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;
//creare structura pentru un nod dintr-un arbore binar de cautare

typedef struct NodArbore NodArbore;

struct NodArbore {
	Masina masina;
	NodArbore* stanga;
	NodArbore* dreapta;
};


Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void adaugaMasinaInArbore(NodArbore** radacina, Masina masinaNoua) {
	//adauga o noua masina pe care o primim ca parametru in arbore,
	//astfel incat sa respecte princiippile de arbore binar de cautare
	//dupa o anumita cheie pe care o decideti - poate fi ID
	if (*radacina == NULL) {
		NodArbore* nodNou = (NodArbore*)malloc(sizeof(NodArbore));
		nodNou->masina = masinaNoua;
		nodNou->stanga = NULL;
		nodNou->dreapta = NULL;

		*radacina = nodNou;
	}
	else {
		if (masinaNoua.id < (*radacina)->masina.id) {
			adaugaMasinaInArbore(&((*radacina)->stanga), masinaNoua);
		}
		else {
			adaugaMasinaInArbore(&((*radacina)->dreapta), masinaNoua);
		}
	}
}

NodArbore* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	NodArbore* arbore = NULL;

	FILE* f = fopen(numeFisier, "r");

	if (f == NULL) {
		printf("Nu s-a putut deschide fisierul: %s\n", numeFisier);
		return arbore;
	}

	while (!feof(f)) {
		Masina masina = citireMasinaDinFisier(f);
		adaugaMasinaInArbore(&arbore, masina);
	}

	fclose(f);

	return arbore;
}

void afisareMasiniDinArbore(/*arbore de masini*/) {
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere
}

void dezalocareArboreDeMasini(/*arbore de masini*/) {
	//sunt dezalocate toate masinile si arborele de elemente
}

Masina getMasinaByID(/*arborele de masini*/int id) {
	Masina m;

	return m;
}

int determinaNumarNoduri(/*arborele de masini*/) {
	//calculeaza numarul total de noduri din arborele binar de cautare
	return 0;
}

int calculeazaInaltimeArbore(/*arbore de masini*/) {
	//calculeaza inaltimea arborelui care este data de 
	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
	return 0;
}

float calculeazaPretTotal(/*arbore de masini*/) {
	//calculeaza pretul tuturor masinilor din arbore.
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(/*arbore de masini*/ const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	return 0;
}

int main() {
	NodArbore* arbore = citireArboreDeMasiniDinFisier("masini.txt");

	if (arbore != NULL) {
		afisareMasina(arbore->masina);
	}

	return 0;
}