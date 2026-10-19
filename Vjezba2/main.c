#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Person* Position;

typedef struct Person {
    char name[30];
    char lastname[30];
    int year;
    Position next;
} Person;


// A) Dodavanje novog elementa na pocetak liste
Position insertBegin(char* name, char* lastname, int year) {
    Position newPerson = (Position)malloc(sizeof(Person));

    if (newPerson == NULL)
        return NULL;

    strcpy(newPerson->name, name);
    strcpy(newPerson->lastname, lastname);
    newPerson->year = year;
    newPerson->next = NULL;

    return newPerson;
}


// Pomocna funkcija za povezivanje elemenata
int insertAfter(Position p, Position newP) {
    if (p == NULL || newP == NULL)
        return -1;

    newP->next = p->next;
    p->next = newP;

    return 0;
}


// B) Ispis liste
void printList(Position p) {
    Position q = p->next;

    if (q == NULL) {
        printf("Lista je prazna.\n");
        return;
    }

    while (q != NULL) {
        printf("%s %s %d\n",
            q->name, q->lastname, q->year);

        q = q->next;
    }
}


// C) Dodavanje novog elementa na kraj liste
int insertEnd(Position p, Position newPerson) {
    Position q;

    if (p == NULL || newPerson == NULL)
        return -1;

    for (q = p; q->next != NULL; q = q->next)
        ;

    return insertAfter(q, newPerson);
}


// D) Pronalaženje osobe po prezimenu
Position findName(char* lastN, Position p) {
    Position q = p->next;

    while (q != NULL) {
        if (strcmp(q->lastname, lastN) == 0)
            return q;

        q = q->next;
    }

    return NULL;
}


// E) Brisanje osobe po prezimenu
int deletePerson(char* lastN, Position p) {
    Position prev = p;
    Position curr;

    if (p == NULL)
        return -1;

    curr = findName(lastN, p);

    if (curr == NULL)
        return -1;

    while (prev->next != curr)
        prev = prev->next;

    prev->next = curr->next;
    free(curr);

    return 0;
}


// Glavni program
int main(void) {
    Person Head = { "", "", 0, NULL };
    Position p = &Head;
    Position newPerson;
    Position found;

    char name[30];
    char lastname[30];
    int year;
    int choice;

    do {
        printf("\n--- IZBORNIK ---\n");
        printf("1 - Dodaj na pocetak (A)\n");
        printf("2 - Ispisi listu (B)\n");
        printf("3 - Dodaj na kraj (C)\n");
        printf("4 - Pronadi osobu (D)\n");
        printf("5 - Obrisi osobu (E)\n");
        printf("0 - Izlaz\n");
        printf("Odabir: ");

        scanf("%d", &choice);

        switch (choice) {
        case 1:
        case 3:
            printf("Ime: ");
            scanf("%29s", name);

            printf("Prezime: ");
            scanf("%29s", lastname);

            printf("Godina rodenja: ");
            scanf("%d", &year);

            newPerson = insertBegin(name, lastname, year);

            if (newPerson == NULL) {
                printf("Greska pri alokaciji memorije.\n");
                break;
            }

            if (choice == 1) {
                insertAfter(p, newPerson);
            }
            else {
                insertEnd(p, newPerson);
            }

            break;

        case 2:
            printList(p);
            break;

        case 4:
            printf("Unesi prezime za pretragu: ");
            scanf("%29s", lastname);

            found = findName(lastname, p);

            if (found != NULL) {
                printf("Pronadena osoba: %s %s, %d\n",
                    found->name,
                    found->lastname,
                    found->year);
            }
            else {
                printf("Osoba nije pronadena.\n");
            }

            break;

        case 5:
            printf("Unesi prezime za brisanje: ");
            scanf("%29s", lastname);

            if (deletePerson(lastname, p) == 0)
                printf("Osoba je obrisana.\n");
            else
                printf("Osoba nije pronadena.\n");

            break;

        case 0:
            printf("Kraj programa.\n");
            break;

        default:
            printf("Neispravan odabir.\n");
        }

    } while (choice != 0);

    // Oslobadanje memorije
    while (Head.next != NULL) {
        Position temp = Head.next;
        Head.next = temp->next;
        free(temp);
    }

    return 0;
}
