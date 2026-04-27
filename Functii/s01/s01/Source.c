#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Sablon {
    int id;
    char* nume;
    float pret;
    char categorie;
};

struct Sablon initializare(int id, const char* nume, float pret, char categorie) {
    struct Sablon s;

    s.id = id;
    s.nume = (char*)malloc(strlen(nume) + 1);
    strcpy(s.nume, nume);
    s.pret = pret;
    s.categorie = categorie;

    return s;
}

void afisare(struct Sablon s) {
    printf("ID: %d\n", s.id);
    printf("Nume: %s\n", s.nume);
    printf("Pret: %.2f\n", s.pret);
    printf("Categorie: %c\n", s.categorie);
}

void modifica_Atribut(struct Sablon* s, float pretNou) {
    if (s != NULL) {
        s->pret = pretNou;
    }
}

void dezalocare(struct Sablon* s) {
    if (s != NULL && s->nume != NULL) {
        free(s->nume);
        s->nume = NULL;
    }
}

int main() {
    struct Sablon s;

    s = initializare(1, "Produs", 99.99f, 'A');

    afisare(s);

    modifica_Atribut(&s, 120.50f);

    printf("\nDupa modificare:\n");
    afisare(s);

    dezalocare(&s);

    return 0;
}