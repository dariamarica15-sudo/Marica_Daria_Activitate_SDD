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

TeamAvatar* citireVectorTeamAvatarFisier(const char* numeFisier, int* nrMembriCititi) {

	FILE* f = fopen(numeFisier, "r");

	if (f == NULL) {
		printf("Fisierul nu a fost gasit\n");
		return NULL;
	}

	TeamAvatar* team = NULL;

	while (!feof(f)) {

		TeamAvatar membru = citireMembruFisier(f);

		if (membru.id != -1) {
			adaugaMembruInVector(&team, nrMembriCititi, membru);
		}
	}

	fclose(f);

	return team;
}



int main() {
	TeamAvatar* team = NULL;
	int nrMembri = 0;

	team = citireVectorTeamAvatarFisier("personaje.txt", &nrMembri);

	afisareVectorTeamAvatar(team, nrMembri);

	for (int i = 0; i < nrMembri; i++) {
		free(team[i].nume);
		free(team[i].element);
	}

	free(team);

	return 0;
}