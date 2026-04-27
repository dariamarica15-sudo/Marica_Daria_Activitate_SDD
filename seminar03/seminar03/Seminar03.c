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

void afisareMasina(Masina masina) {
    printf("\nDate masina:");
    printf("\nId: %d", masina.id);
    printf("\nNumar usi: %d", masina.nrUsi);
    printf("\nPret: %5.2f", masina.pret);
    printf("\nModel: %s", masina.model);
    printf("\nNume sofer: %s", masina.numeSofer);
    printf("\nSeria: %c\n", masina.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
    for (int i = 0; i < nrMasini; i++) {
        afisareMasina(masini[i]);
    }
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
    Masina* vectorNou = (Masina*)malloc(sizeof(Masina) * ((*nrMasini) + 1));

    for (int i = 0; i < (*nrMasini); i++) {
        vectorNou[i] = (*masini)[i];
    }

    vectorNou[*nrMasini] = masinaNoua;

    free(*masini);
    *masini = vectorNou;
    (*nrMasini)++;
}

Masina citireMasinaDinLinie(char* buffer) {
    Masina m;
    char separator[] = ",\n";

    char* token = strtok(buffer, separator);
    m.id = atoi(token);

    token = strtok(NULL, separator);
    m.nrUsi = atoi(token);

    token = strtok(NULL, separator);
    m.pret = (float)atof(token);

    token = strtok(NULL, separator);
    m.model = (char*)malloc(strlen(token) + 1);
    strcpy(m.model, token);

    token = strtok(NULL, separator);
    m.numeSofer = (char*)malloc(strlen(token) + 1);
    strcpy(m.numeSofer, token);

    token = strtok(NULL, separator);
    m.serie = token[0];

    return m;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
    FILE* f = fopen(numeFisier, "r");

    if (f == NULL) {
        printf("Nu s-a putut deschide fisierul %s\n", numeFisier);
        *nrMasiniCitite = 0;
        return NULL;
    }

    Masina* masini = NULL;
    *nrMasiniCitite = 0;

    char buffer[100];

    while (fgets(buffer, sizeof(buffer), f) != NULL) {
        if (strlen(buffer) > 1) {
            Masina m = citireMasinaDinLinie(buffer);
            adaugaMasinaInVector(&masini, nrMasiniCitite, m);
        }
    }

    fclose(f);
    return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
    for (int i = 0; i < (*nrMasini); i++) {
        free((*vector)[i].model);
        free((*vector)[i].numeSofer);
    }

    free(*vector);
    *vector = NULL;
    *nrMasini = 0;
}

int main() {
    int nrMasini = 0;

    Masina* masini = citireVectorMasiniFisier("masini.txt", &nrMasini);

    afisareVectorMasini(masini, nrMasini);

    dezalocareVectorMasini(&masini, &nrMasini);

    return 0;
}