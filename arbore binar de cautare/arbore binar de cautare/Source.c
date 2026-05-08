#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

	Masina m1;
	m1.id = -1;
	m1.model = NULL;
	m1.numeSofer = NULL;

	if (fgets(buffer, 100, file) == NULL) {
		return m1;
	}

	char* aux;

	aux = strtok(buffer, sep);
	if (aux == NULL) {
		return m1;
	}
	m1.id = atoi(aux);

	aux = strtok(NULL, sep);
	m1.nrUsi = atoi(aux);

	aux = strtok(NULL, sep);
	m1.pret = atof(aux);

	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.serie = *aux;

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

void adaugaMasinaInArbore(NodArbore** arbore, Masina masinaNoua) {
	//adauga o noua masina pe care o primim ca parametru in arbore,
	//astfel incat sa respecte princiippile de arbore binar de cautare
	//dupa o anumita cheie pe care o decideti - poate fi ID

	if (*arbore == NULL) {
		NodArbore* nodNou = malloc(sizeof(NodArbore));

		nodNou->masina = masinaNoua;
		nodNou->stanga = NULL;
		nodNou->dreapta = NULL;

		*arbore = nodNou;
	}
	else {
		if (masinaNoua.id < (*arbore)->masina.id) {
			adaugaMasinaInArbore(&((*arbore)->stanga), masinaNoua);
		}
		else {
			adaugaMasinaInArbore(&((*arbore)->dreapta), masinaNoua);
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

		if (masina.id != -1) {
			adaugaMasinaInArbore(&arbore, masina);
		}
	}

	fclose(f);

	return arbore;
}

void afisarePreordine(NodArbore* arbore) {
	if (arbore != NULL) {
		afisareMasina(arbore->masina);
		afisarePreordine(arbore->stanga);
		afisarePreordine(arbore->dreapta);
	}
}

void afisareInordine(NodArbore* arbore) {
	if (arbore != NULL) {
		afisareInordine(arbore->stanga);
		afisareMasina(arbore->masina);
		afisareInordine(arbore->dreapta);
	}
}

void afisarePostordine(NodArbore* arbore) {
	if (arbore != NULL) {
		afisarePostordine(arbore->stanga);
		afisarePostordine(arbore->dreapta);
		afisareMasina(arbore->masina);
	}
}

void afisareMasiniDinArbore(NodArbore* arbore) {
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere

	printf("Afisare preordine:\n");
	afisarePreordine(arbore);

	printf("Afisare inordine:\n");
	afisareInordine(arbore);

	printf("Afisare postordine:\n");
	afisarePostordine(arbore);
}

void dezalocareMasina(Masina* masina) {
	free(masina->model);
	free(masina->numeSofer);

	masina->model = NULL;
	masina->numeSofer = NULL;
}

void dezalocareArboreDeMasini(NodArbore** arbore) {
	//sunt dezalocate toate masinile si arborele de elemente

	if (*arbore != NULL) {
		dezalocareArboreDeMasini(&((*arbore)->stanga));
		dezalocareArboreDeMasini(&((*arbore)->dreapta));

		dezalocareMasina(&((*arbore)->masina));
		free(*arbore);

		*arbore = NULL;
	}
}

Masina getMasinaByID(NodArbore* arbore, int id) {
	Masina m;
	m.id = -1;
	m.model = NULL;
	m.numeSofer = NULL;

	if (arbore == NULL) {
		return m;
	}

	if (arbore->masina.id == id) {
		m.id = arbore->masina.id;
		m.nrUsi = arbore->masina.nrUsi;
		m.pret = arbore->masina.pret;
		m.serie = arbore->masina.serie;

		m.model = malloc(strlen(arbore->masina.model) + 1);
		strcpy_s(m.model, strlen(arbore->masina.model) + 1, arbore->masina.model);

		m.numeSofer = malloc(strlen(arbore->masina.numeSofer) + 1);
		strcpy_s(m.numeSofer, strlen(arbore->masina.numeSofer) + 1, arbore->masina.numeSofer);

		return m;
	}
	else if (id < arbore->masina.id) {
		return getMasinaByID(arbore->stanga, id);
	}
	else {
		return getMasinaByID(arbore->dreapta, id);
	}
}

int determinaNumarNoduri(NodArbore* arbore) {
	//calculeaza numarul total de noduri din arborele binar de cautare

	if (arbore == NULL) {
		return 0;
	}

	return 1 + determinaNumarNoduri(arbore->stanga) + determinaNumarNoduri(arbore->dreapta);
}

int calculeazaInaltimeArbore(NodArbore* arbore) {
	//calculeaza inaltimea arborelui care este data de 
	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza

	if (arbore == NULL) {
		return 0;
	}

	int inaltimeStanga = calculeazaInaltimeArbore(arbore->stanga);
	int inaltimeDreapta = calculeazaInaltimeArbore(arbore->dreapta);

	if (inaltimeStanga > inaltimeDreapta) {
		return 1 + inaltimeStanga;
	}
	else {
		return 1 + inaltimeDreapta;
	}
}

float calculeazaPretTotal(NodArbore* arbore) {
	//calculeaza pretul tuturor masinilor din arbore.

	if (arbore == NULL) {
		return 0;
	}

	return arbore->masina.pret +
		calculeazaPretTotal(arbore->stanga) +
		calculeazaPretTotal(arbore->dreapta);
}

float calculeazaPretulMasinilorUnuiSofer(NodArbore* arbore, const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.

	if (arbore == NULL) {
		return 0;
	}

	float suma = 0;

	if (strcmp(arbore->masina.numeSofer, numeSofer) == 0) {
		suma += arbore->masina.pret;
	}

	suma += calculeazaPretulMasinilorUnuiSofer(arbore->stanga, numeSofer);
	suma += calculeazaPretulMasinilorUnuiSofer(arbore->dreapta, numeSofer);

	return suma;
}

int main() {
	NodArbore* arbore = citireArboreDeMasiniDinFisier("masini.txt");

	afisareMasiniDinArbore(arbore);

	printf("Numar noduri: %d\n", determinaNumarNoduri(arbore));
	printf("Inaltime arbore: %d\n", calculeazaInaltimeArbore(arbore));
	printf("Pret total: %.2f\n", calculeazaPretTotal(arbore));
	printf("Pret masini sofer Ionescu: %.2f\n", calculeazaPretulMasinilorUnuiSofer(arbore, "Ionescu"));

	Masina masina = getMasinaByID(arbore, 5);

	printf("\nMasina cautata dupa ID:\n");
	if (masina.id != -1) {
		afisareMasina(masina);
		dezalocareMasina(&masina);
	}
	else {
		printf("Masina nu a fost gasita.\n");
	}

	dezalocareArboreDeMasini(&arbore);

	return 0;
}