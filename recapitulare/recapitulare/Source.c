#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraTeamAvatar {
	int id;
	int varsta;
	float putere;
	char* nume;
	char* element;
	unsigned char initiala;
};

typedef struct StructuraTeamAvatar TeamAvatar;

void afisareMembruTeamAvatar(TeamAvatar membru) {
	printf("Id: %d\n", membru.id);
	printf("Varsta: %d\n", membru.varsta);
	printf("Putere: %.2f\n", membru.putere);
	printf("Nume: %s\n", membru.nume);
	printf("Element: %s\n", membru.element);
	printf("Initiala: %c\n\n", membru.initiala);
}

void afisareVectorTeamAvatar(TeamAvatar* team, int nrMembri) {
	for (int i = 0; i < nrMembri; i++) {
		afisareMembruTeamAvatar(team[i]);
	}
}

TeamAvatar citireMembruFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	char* aux;

	TeamAvatar membru;
	membru.id = -1;

	if (fgets(buffer, 100, file) == NULL) {
		return membru;
	}

	aux = strtok(buffer, sep);
	membru.id = atoi(aux);

	aux = strtok(NULL, sep);
	membru.nume = (char*)malloc(strlen(aux) + 1);
	strcpy(membru.nume, aux);

	aux = strtok(NULL, sep);
	membru.element = (char*)malloc(strlen(aux) + 1);
	strcpy(membru.element, aux);

	aux = strtok(NULL, sep);
	membru.varsta = atoi(aux);

	aux = strtok(NULL, sep);
	membru.putere = (float)atof(aux);

	aux = strtok(NULL, sep);
	membru.initiala = aux[0];

	return membru;
}

void adaugaMembruInVector(TeamAvatar** team, int* nrMembri, TeamAvatar membruNou) {
	TeamAvatar* vectorNou = (TeamAvatar*)malloc(sizeof(TeamAvatar) * ((*nrMembri) + 1));

	for (int i = 0; i < *nrMembri; i++) {
		vectorNou[i] = (*team)[i];
	}

	vectorNou[*nrMembri] = membruNou;

	free(*team);

	*team = vectorNou;
	(*nrMembri)++;
}

int main() {
	TeamAvatar* team = NULL;
	int nrMembri = 0;

	FILE* f = fopen("personaje.txt", "r");

	if (f == NULL) {
		printf("Fisierul nu a fost gasit\n");
		return 0;
	}

	TeamAvatar membru = citireMembruFisier(f);

	adaugaMembruInVector(&team, &nrMembri, membru);

	afisareVectorTeamAvatar(team, nrMembri);

	free(team[0].nume);
	free(team[0].element);
	free(team);

	fclose(f);

	return 0;
}