#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Jucator {
    int id;
    int varsta;
    char* nume;
    char* tara;
    int nrTrofee;
} Jucator;

typedef struct NodArbore NodArbore;

struct NodArbore {
    Jucator info;
    NodArbore* stanga;
    NodArbore* dreapta;
};

typedef struct NodStiva NodStiva;

struct NodStiva {
    Jucator info;
    NodStiva* next;
};

Jucator copiazaJucator(Jucator j) {
    Jucator nou;

    nou.id = j.id;
    nou.varsta = j.varsta;
    nou.nrTrofee = j.nrTrofee;

    nou.nume = malloc(strlen(j.nume) + 1);
    strcpy(nou.nume, j.nume);

    nou.tara = malloc(strlen(j.tara) + 1);
    strcpy(nou.tara, j.tara);

    return nou;
}

void afisareJucator(Jucator j) {
    if (j.id == -1) {
        printf("Jucatorul nu exista.\n\n");
        return;
    }

    printf("Id: %d\n", j.id);
    printf("Varsta: %d\n", j.varsta);
    printf("Nume: %s\n", j.nume);
    printf("Tara: %s\n", j.tara);
    printf("Numar trofee: %d\n\n", j.nrTrofee);
}

Jucator citireJucatorDinFisier(FILE* f) {
    char buffer[100];
    char sep[] = ",\n";
    Jucator j;

    fgets(buffer, 100, f);

    char* aux = strtok(buffer, sep);
    j.id = atoi(aux);

    aux = strtok(NULL, sep);
    j.varsta = atoi(aux);

    aux = strtok(NULL, sep);
    j.nume = malloc(strlen(aux) + 1);
    strcpy(j.nume, aux);

    aux = strtok(NULL, sep);
    j.tara = malloc(strlen(aux) + 1);
    strcpy(j.tara, aux);

    aux = strtok(NULL, sep);
    j.nrTrofee = atoi(aux);

    return j;
}

void inserareJucatorArbore(NodArbore** radacina, Jucator j) {
    if (*radacina == NULL) {
        NodArbore* nou = malloc(sizeof(NodArbore));
        nou->info = j;
        nou->stanga = NULL;
        nou->dreapta = NULL;

        *radacina = nou;
    }
    else {
        if (j.id < (*radacina)->info.id) {
            inserareJucatorArbore(&((*radacina)->stanga), j);
        }
        else {
            inserareJucatorArbore(&((*radacina)->dreapta), j);
        }
    }
}

NodArbore* citireArboreDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    NodArbore* radacina = NULL;

    if (f == NULL) {
        printf("Fisierul nu a fost gasit.\n");
        return NULL;
    }

    while (!feof(f)) {
        Jucator j = citireJucatorDinFisier(f);
        inserareJucatorArbore(&radacina, j);
    }

    fclose(f);
    return radacina;
}

void parcurgereInordine(NodArbore* radacina) {
    if (radacina != NULL) {
        parcurgereInordine(radacina->stanga);
        afisareJucator(radacina->info);
        parcurgereInordine(radacina->dreapta);
    }
}

int numarJucatoriDinTara(NodArbore* radacina, const char* tara) {
    if (radacina == NULL) {
        return 0;
    }

    int nr = 0;

    if (strcmp(radacina->info.tara, tara) == 0) {
        nr = 1;
    }

    return nr
        + numarJucatoriDinTara(radacina->stanga, tara)
        + numarJucatoriDinTara(radacina->dreapta, tara);
}

Jucator cautaJucatorDupaId(NodArbore* radacina, int id) {
    Jucator j;
    j.id = -1;
    j.nume = NULL;
    j.tara = NULL;
    j.varsta = 0;
    j.nrTrofee = 0;

    if (radacina == NULL) {
        return j;
    }

    if (radacina->info.id == id) {
        return copiazaJucator(radacina->info);
    }

    if (id < radacina->info.id) {
        return cautaJucatorDupaId(radacina->stanga, id);
    }
    else {
        return cautaJucatorDupaId(radacina->dreapta, id);
    }
}

void push(NodStiva** stiva, Jucator j) {
    NodStiva* nou = malloc(sizeof(NodStiva));
    nou->info = j;
    nou->next = *stiva;

    *stiva = nou;
}

Jucator pop(NodStiva** stiva) {
    Jucator j;
    j.id = -1;

    if (*stiva == NULL) {
        return j;
    }

    NodStiva* aux = *stiva;
    j = aux->info;
    *stiva = aux->next;

    free(aux);

    return j;
}

void salveazaInStivaCrescator(NodArbore* radacina, NodStiva** stiva) {
    if (radacina != NULL) {
        salveazaInStivaCrescator(radacina->dreapta, stiva);
        push(stiva, copiazaJucator(radacina->info));
        salveazaInStivaCrescator(radacina->stanga, stiva);
    }
}

void afisareStiva(NodStiva** stiva) {
    while (*stiva != NULL) {
        Jucator j = pop(stiva);
        afisareJucator(j);

        free(j.nume);
        free(j.tara);
    }
}

int main() {
    NodArbore* arbore = citireArboreDinFisier("jucatori.txt");

    printf("Afisare inordine \n");
    parcurgereInordine(arbore);

    printf("Numar jucatori din Serbia \n");
    int nr = numarJucatoriDinTara(arbore, "Serbia");
    printf("Numar jucatori din Serbia: %d\n\n", nr);

    printf("Cautare jucator dupa id: \n");
    Jucator j = cautaJucatorDupaId(arbore, 4);
    afisareJucator(j);

    if (j.id != -1) {
        free(j.nume);
        free(j.tara);
    }

    printf("Afisare stiva crescator dupa id: \n");
    NodStiva* stiva = NULL;
    salveazaInStivaCrescator(arbore, &stiva);
    afisareStiva(&stiva);

    return 0;
}