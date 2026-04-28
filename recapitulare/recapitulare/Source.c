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

int main() {
	TeamAvatar membru;

	membru.id = 1;
	membru.varsta = 12;
	membru.putere = 100.5;

	membru.nume = (char*)malloc(strlen("Aang") + 1);
	strcpy(membru.nume, "Aang");

	membru.element = (char*)malloc(strlen("Air") + 1);
	strcpy(membru.element, "Air");

	membru.initiala = 'A';

	afisareMembruTeamAvatar(membru);

	free(membru.nume);
	free(membru.element);

	return 0;
}