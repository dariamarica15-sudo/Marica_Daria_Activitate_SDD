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
	Masina info;
	NodArbore* stanga;
	NodArbore* dreapta;
	int inaltime;
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
	m1.pret = atof(strtok(NULL, sep));

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

//int calculeazaInaltimeArbore(/*arbore de masini*/) {
//	//calculeaza inaltimea arborelui care este data de 
//	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
//	return 0;
//}

int calculeazaInaltimeArbore(NodArbore* arbore) {
	if (arbore == NULL) {
		return 0;
	}
	return arbore->inaltime;
}

//ALTE FUNCTII NECESARE:
// - aici veti adauga noile functii de care aveti nevoie.

int maxim(int a, int b) {
	return a > b ? a : b;
}

NodArbore* creareNod(Masina masinaNoua) {
	NodArbore* nod = malloc(sizeof(NodArbore));
	nod->info = masinaNoua;
	nod->stanga = NULL;
	nod->dreapta = NULL;
	nod->inaltime = 1;
	return nod;
}

int factorEchilibru(NodArbore* arbore) {
	if (arbore == NULL) {
		return 0;
	}
	return calculeazaInaltimeArbore(arbore->stanga) -
		calculeazaInaltimeArbore(arbore->dreapta);
}

NodArbore* rotireDreapta(NodArbore* y) {
	NodArbore* x = y->stanga;
	NodArbore* T2 = x->dreapta;

	x->dreapta = y;
	y->stanga = T2;

	y->inaltime = 1 + maxim(calculeazaInaltimeArbore(y->stanga),
		calculeazaInaltimeArbore(y->dreapta));

	x->inaltime = 1 + maxim(calculeazaInaltimeArbore(x->stanga),
		calculeazaInaltimeArbore(x->dreapta));

	return x;
}

NodArbore* rotireStanga(NodArbore* x) {
	NodArbore* y = x->dreapta;
	NodArbore* T2 = y->stanga;

	y->stanga = x;
	x->dreapta = T2;

	x->inaltime = 1 + maxim(calculeazaInaltimeArbore(x->stanga),
		calculeazaInaltimeArbore(x->dreapta));

	y->inaltime = 1 + maxim(calculeazaInaltimeArbore(y->stanga),
		calculeazaInaltimeArbore(y->dreapta));

	return y;
}

NodArbore* adaugaMasinaInArboreEchilibrat(NodArbore* arbore, Masina masinaNoua) {
	//adauga o noua masina pe care o primim ca parametru in arbore,
	//astfel incat sa respecte principiile de arbore binar de cautare ECHILIBRAT
	//dupa o anumita cheie pe care o decideti - poate fi ID

	if (arbore == NULL) {
		return creareNod(masinaNoua);
	}

	if (masinaNoua.id < arbore->info.id) {
		arbore->stanga = adaugaMasinaInArboreEchilibrat(arbore->stanga, masinaNoua);
	}
	else if (masinaNoua.id > arbore->info.id) {
		arbore->dreapta = adaugaMasinaInArboreEchilibrat(arbore->dreapta, masinaNoua);
	}
	else {
		return arbore;
	}

	arbore->inaltime = 1 + maxim(calculeazaInaltimeArbore(arbore->stanga),
		calculeazaInaltimeArbore(arbore->dreapta));

	int echilibru = factorEchilibru(arbore);

	if (echilibru > 1 && masinaNoua.id < arbore->stanga->info.id) {
		return rotireDreapta(arbore);
	}

	if (echilibru < -1 && masinaNoua.id > arbore->dreapta->info.id) {
		return rotireStanga(arbore);
	}

	if (echilibru > 1 && masinaNoua.id > arbore->stanga->info.id) {
		arbore->stanga = rotireStanga(arbore->stanga);
		return rotireDreapta(arbore);
	}

	if (echilibru < -1 && masinaNoua.id < arbore->dreapta->info.id) {
		arbore->dreapta = rotireDreapta(arbore->dreapta);
		return rotireStanga(arbore);
	}

	return arbore;
}

void* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul

	FILE* f = fopen(numeFisier, "r");
	NodArbore* arbore = NULL;

	if (f == NULL) {
		printf("Fisierul nu s-a putut deschide.\n");
		return NULL;
	}

	while (!feof(f)) {
		Masina masina = citireMasinaDinFisier(f);
		arbore = adaugaMasinaInArboreEchilibrat(arbore, masina);
	}

	fclose(f);
	return arbore;
}

void afisareInordine(NodArbore* arbore) {
	if (arbore != NULL) {
		afisareInordine(arbore->stanga);
		afisareMasina(arbore->info);
		afisareInordine(arbore->dreapta);
	}
}

void afisarePreordine(NodArbore* arbore) {
	if (arbore != NULL) {
		afisareMasina(arbore->info);
		afisarePreordine(arbore->stanga);
		afisarePreordine(arbore->dreapta);
	}
}

void afisarePostordine(NodArbore* arbore) {
	if (arbore != NULL) {
		afisarePostordine(arbore->stanga);
		afisarePostordine(arbore->dreapta);
		afisareMasina(arbore->info);
	}
}

void afisareMasiniDinArbore(NodArbore* arbore) {
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere

	printf("\n--- Afisare INORDINE ---\n");
	afisareInordine(arbore);

	printf("\n--- Afisare PREORDINE ---\n");
	afisarePreordine(arbore);

	printf("\n--- Afisare POSTORDINE ---\n");
	afisarePostordine(arbore);
}

void dezalocareArboreDeMasini(NodArbore** arbore) {
	//sunt dezalocate toate masinile si arborele de elemente

	if (*arbore != NULL) {
		dezalocareArboreDeMasini(&((*arbore)->stanga));
		dezalocareArboreDeMasini(&((*arbore)->dreapta));

		free((*arbore)->info.model);
		free((*arbore)->info.numeSofer);
		free(*arbore);

		*arbore = NULL;
	}
}

//Preluati urmatoarele functii din laboratorul precedent.
//Acestea ar trebuie sa functioneze pe noul arbore echilibrat.

Masina getMasinaByID(NodArbore* arbore, int id) {
	Masina masina;
	masina.id = -1;
	masina.model = NULL;
	masina.numeSofer = NULL;

	if (arbore == NULL) {
		return masina;
	}

	if (id == arbore->info.id) {
		masina.id = arbore->info.id;
		masina.nrUsi = arbore->info.nrUsi;
		masina.pret = arbore->info.pret;
		masina.serie = arbore->info.serie;

		masina.model = malloc(strlen(arbore->info.model) + 1);
		strcpy_s(masina.model, strlen(arbore->info.model) + 1, arbore->info.model);

		masina.numeSofer = malloc(strlen(arbore->info.numeSofer) + 1);
		strcpy_s(masina.numeSofer, strlen(arbore->info.numeSofer) + 1, arbore->info.numeSofer);

		return masina;
	}

	if (id < arbore->info.id) {
		return getMasinaByID(arbore->stanga, id);
	}
	else {
		return getMasinaByID(arbore->dreapta, id);
	}
}

int determinaNumarNoduri(NodArbore* arbore) {
	if (arbore == NULL) {
		return 0;
	}

	return 1 + determinaNumarNoduri(arbore->stanga)
		+ determinaNumarNoduri(arbore->dreapta);
}

float calculeazaPretTotal(NodArbore* arbore) {
	if (arbore == NULL) {
		return 0;
	}

	return arbore->info.pret
		+ calculeazaPretTotal(arbore->stanga)
		+ calculeazaPretTotal(arbore->dreapta);
}

float calculeazaPretulMasinilorUnuiSofer(NodArbore* arbore, const char* numeSofer) {
	if (arbore == NULL) {
		return 0;
	}

	float suma = 0;

	if (strcmp(arbore->info.numeSofer, numeSofer) == 0) {
		suma += arbore->info.pret;
	}

	suma += calculeazaPretulMasinilorUnuiSofer(arbore->stanga, numeSofer);
	suma += calculeazaPretulMasinilorUnuiSofer(arbore->dreapta, numeSofer);

	return suma;
}

int main() {

	NodArbore* arbore = NULL;

	arbore = (NodArbore*)citireArboreDeMasiniDinFisier("masini.txt");

	afisareMasiniDinArbore(arbore);

	printf("\nInaltime arbore: %d", calculeazaInaltimeArbore(arbore));
	printf("\nNumar noduri: %d", determinaNumarNoduri(arbore));
	printf("\nPret total masini: %.2f", calculeazaPretTotal(arbore));

	printf("\n\nMasina cautata dupa ID:\n");
	Masina masinaCautata = getMasinaByID(arbore, 2);
	afisareMasina(masinaCautata);

	if (masinaCautata.id != -1) {
		free(masinaCautata.model);
		free(masinaCautata.numeSofer);
	}

	printf("\nPret total masini pentru soferul Ion: %.2f",
		calculeazaPretulMasinilorUnuiSofer(arbore, "Ion"));

	dezalocareArboreDeMasini(&arbore);

	return 0;
}