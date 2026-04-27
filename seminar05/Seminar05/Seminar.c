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

typedef struct Nod {
    Masina info;
    struct Nod* next;
    struct Nod* prev;
} Nod;

typedef struct ListaDubla {
    Nod* prim;
    Nod* ultim;
} ListaDubla;

Masina citireMasinaDinFisier(FILE* file) {
    char buffer[100];
    char sep[] = ",\n";

    fgets(buffer, 100, file);

    Masina m1;

    char* aux = strtok(buffer, sep);
    m1.id = atoi(aux);

    aux = strtok(NULL, sep);
    m1.nrUsi = atoi(aux);

    aux = strtok(NULL, sep);
    m1.pret = (float)atof(aux);

    aux = strtok(NULL, sep);
    m1.model = (char*)malloc(strlen(aux) + 1);
    strcpy(m1.model, aux);

    aux = strtok(NULL, sep);
    m1.numeSofer = (char*)malloc(strlen(aux) + 1);
    strcpy(m1.numeSofer, aux);

    aux = strtok(NULL, sep);
    m1.serie = aux[0];

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

void afisareListaMasini(ListaDubla lista) {
    Nod* p = lista.prim;

    while (p) {
        afisareMasina(p->info);
        p = p->next;
    }
}

void afisareInversaListaMasini(ListaDubla lista) {
    Nod* p = lista.ultim;

    while (p) {
        afisareMasina(p->info);
        p = p->prev;
    }
}

void adaugaMasinaInLista(ListaDubla* lista, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));

    nou->info = masinaNoua;
    nou->next = NULL;
    nou->prev = lista->ultim;

    if (lista->ultim) {
        lista->ultim->next = nou;
    }
    else {
        lista->prim = nou;
    }

    lista->ultim = nou;
}

void adaugaLaInceputInLista(ListaDubla* lista, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));

    nou->info = masinaNoua;
    nou->next = lista->prim;
    nou->prev = NULL;

    if (lista->prim) {
        lista->prim->prev = nou;
    }
    else {
        lista->ultim = nou;
    }

    lista->prim = nou;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
    ListaDubla ld;
    ld.prim = NULL;
    ld.ultim = NULL;

    FILE* f = fopen(numeFisier, "r");

    if (f == NULL) {
        printf("Nu s-a putut deschide fisierul %s\n", numeFisier);
        return ld;
    }

    char buffer[100];

    while (fgets(buffer, sizeof(buffer), f) != NULL) {
        FILE* temp = tmpfile();
        fputs(buffer, temp);
        rewind(temp);

        Masina m = citireMasinaDinFisier(temp);
        adaugaMasinaInLista(&ld, m);

        fclose(temp);
    }

    fclose(f);
    return ld;
}

void dezalocareLDMasini(ListaDubla* ld) {
    Nod* p = ld->prim;

    while (p) {
        Nod* next = p->next;

        free(p->info.model);
        free(p->info.numeSofer);
        free(p);

        p = next;
    }

    ld->prim = NULL;
    ld->ultim = NULL;
}

float calculeazaPretMediu(ListaDubla lista) {
    float sum = 0;
    int nr = 0;

    Nod* p = lista.prim;

    while (p) {
        sum += p->info.pret;
        nr++;
        p = p->next;
    }

    if (nr == 0) {
        return 0;
    }

    return sum / nr;
}

void stergeMasinaDupaID(ListaDubla* ld, int id) {
    Nod* p = ld->prim;

    while (p && p->info.id != id) {
        p = p->next;
    }

    if (p == NULL) {
        return;
    }

    if (p->prev) {
        p->prev->next = p->next;
    }
    else {
        ld->prim = p->next;
    }

    if (p->next) {
        p->next->prev = p->prev;
    }
    else {
        ld->ultim = p->prev;
    }

    free(p->info.model);
    free(p->info.numeSofer);
    free(p);
}

char* getNumeSoferMasinaScumpa(ListaDubla ld) {
    float mx = -1;
    Nod* p = ld.prim;
    char* nume = NULL;

    while (p) {
        if (p->info.pret > mx) {
            mx = p->info.pret;

            free(nume);

            nume = (char*)malloc(strlen(p->info.numeSofer) + 1);
            strcpy(nume, p->info.numeSofer);
        }

        p = p->next;
    }

    return nume;
}


int main() {
    ListaDubla lista = citireLDMasiniDinFisier(
        "\\Users\\WINDOWS\\Documents\\visual studio 2022 proiecte\\StructuriDeDate\\Seminar04\\Seminar04\\masini.txt"
    );

    afisareListaMasini(lista);

    stergeMasinaDupaID(&lista, 1);

    printf("\nDupa stergere:\n");
    afisareListaMasini(lista);

    printf("\nPret mediu: %.2f\n", calculeazaPretMediu(lista));

    char* nume = getNumeSoferMasinaScumpa(lista);

    if (nume) {
        printf("\nSoferul cu masina cea mai scumpa: %s\n", nume);
        free(nume);
    }

    dezalocareLDMasini(&lista);

    return 0;
}