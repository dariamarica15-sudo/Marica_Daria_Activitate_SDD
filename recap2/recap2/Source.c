#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Consola {
    int id;
    float pret;
    char* producator;
    char* model;
    int anLansare;
} Consola;

typedef struct Heap {
    int lungime;
    int nrConsole;
    Consola* vector;
} Heap;

Consola citireConsolaDinFisier(FILE* f) {
    char buffer[100];
    char sep[] = ",\n";
    Consola c;

    fgets(buffer, 100, f);

    char* aux = strtok(buffer, sep);
    c.id = atoi(aux);

    aux = strtok(NULL, sep);
    c.pret = atof(aux);

    aux = strtok(NULL, sep);
    c.producator = malloc(strlen(aux) + 1);
    strcpy(c.producator, aux);

    aux = strtok(NULL, sep);
    c.model = malloc(strlen(aux) + 1);
    strcpy(c.model, aux);

    aux = strtok(NULL, sep);
    c.anLansare = atoi(aux);

    return c;
}

void afisareConsola(Consola c) {
    if (c.id == -1) {
        printf("Consola nu exista.\n\n");
        return;
    }

    printf("Id: %d\n", c.id);
    printf("Pret: %.2f\n", c.pret);
    printf("Producator: %s\n", c.producator);
    printf("Model: %s\n", c.model);
    printf("An lansare: %d\n\n", c.anLansare);
}

Consola copiazaConsola(Consola c) {
    Consola copie;

    copie.id = c.id;
    copie.pret = c.pret;
    copie.anLansare = c.anLansare;

    copie.producator = malloc(strlen(c.producator) + 1);
    strcpy(copie.producator, c.producator);

    copie.model = malloc(strlen(c.model) + 1);
    strcpy(copie.model, c.model);

    return copie;
}

Heap initializareHeap(int lungime) {
    Heap h;
    h.lungime = lungime;
    h.nrConsole = 0;
    h.vector = malloc(sizeof(Consola) * lungime);
    return h;
}

void filtreazaHeap(Heap h, int pozitie) {
    if (pozitie >= 0 && pozitie < h.nrConsole) {
        int pozMax = pozitie;
        int stanga = 2 * pozitie + 1;
        int dreapta = 2 * pozitie + 2;

        if (stanga < h.nrConsole &&
            h.vector[stanga].pret > h.vector[pozMax].pret) {
            pozMax = stanga;
        }

        if (dreapta < h.nrConsole &&
            h.vector[dreapta].pret > h.vector[pozMax].pret) {
            pozMax = dreapta;
        }

        if (pozMax != pozitie) {
            Consola aux = h.vector[pozitie];
            h.vector[pozitie] = h.vector[pozMax];
            h.vector[pozMax] = aux;

            filtreazaHeap(h, pozMax);
        }
    }
}

Heap citireHeapDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    Heap h = initializareHeap(10);

    if (f == NULL) {
        printf("Fisierul nu exista.\n");
        return h;
    }

    while (!feof(f)) {
        h.vector[h.nrConsole++] = citireConsolaDinFisier(f);
    }

    fclose(f);

    for (int i = (h.nrConsole - 2) / 2; i >= 0; i--) {
        filtreazaHeap(h, i);
    }

    return h;
}

void afisareHeap(Heap h) {
    for (int i = 0; i < h.nrConsole; i++) {
        afisareConsola(h.vector[i]);
    }
}

Consola extrageConsolaMaxima(Heap* h) {
    Consola c;
    c.id = -1;

    if (h->nrConsole <= 0) {
        return c;
    }

    Consola aux = h->vector[0];
    h->vector[0] = h->vector[h->nrConsole - 1];
    h->vector[h->nrConsole - 1] = aux;

    h->nrConsole--;

    for (int i = (h->nrConsole - 2) / 2; i >= 0; i--) {
        filtreazaHeap(*h, i);
    }

    return h->vector[h->nrConsole];
}

float calculeazaPretMediu(Heap h) {
    float suma = 0;

    if (h.nrConsole == 0) {
        return 0;
    }

    for (int i = 0; i < h.nrConsole; i++) {
        suma += h.vector[i].pret;
    }

    return suma / h.nrConsole;
}

Consola cautaConsolaDupaId(Heap h, int id) {
    Consola rezultat;
    rezultat.id = -1;
    rezultat.producator = NULL;
    rezultat.model = NULL;

    for (int i = 0; i < h.nrConsole; i++) {
        if (h.vector[i].id == id) {
            rezultat = copiazaConsola(h.vector[i]);
            return rezultat;
        }
    }

    return rezultat;
}


int main() {
    Heap h = citireHeapDinFisier("console.txt");

    printf("Afisare Heap \n");
    afisareHeap(h);

    printf("Consola cu pret maxim \n");
    Consola maxima = extrageConsolaMaxima(&h);
    afisareConsola(maxima);

    printf("Pret mediu \n");
    float medie = calculeazaPretMediu(h);
    printf("Pretul mediu este: %.2f\n\n", medie);

    printf("Cautare consola dupa id \n");
    Consola cautata = cautaConsolaDupaId(h, 3);
    afisareConsola(cautata);

    if (cautata.id != -1) {
        free(cautata.producator);
        free(cautata.model);
    }

    return 0;
}