#include <stdio.h>
#include <stdlib.h>

// Pokazivac na strukturu Poly
typedef struct Poly* Position;

// Struktura jednog clana polinoma
typedef struct Poly {
    int coeff;       // Koeficijent
    int pow;         // Eksponent
    Position next;   // Pokazivac na sljedeci clan
} Poly;


// Deklaracije funkcija
int ReadFromFile(Position head, const char* filename);
int InsertSorted(Position head, int coeff, int pow);
int AddPolynomials(Position result, Position p1, Position p2);
int MultiplyPolynomials(Position result, Position p1, Position p2);
int Print(Position head);
void DeleteList(Position head);


// Umetanje clana u polinom silazno po eksponentima
int InsertSorted(Position head, int coeff, int pow) {

    if (coeff == 0)
        return 0;  // Clan s koeficijentom 0 ne dodajemo

    Position newNode = (Position)malloc(sizeof(Poly));
    // Alociramo memoriju za novi clan

    if (newNode == NULL)
        return -1;  // Provjera uspjesne alokacije

    newNode->coeff = coeff;
    newNode->pow = pow;
    newNode->next = NULL;
    // Upisujemo podatke u novi cvor

    Position temp = head;  // Pomocni pokazivac na pocetak liste

    while (temp->next != NULL && temp->next->pow > pow) {
        temp = temp->next;
        // Pomicemo se dok ne pronademo odgovarajuce mjesto
    }

    // Provjeravamo postoji li vec clan s istim eksponentom
    if (temp->next != NULL && temp->next->pow == pow) {

        temp->next->coeff += coeff;
        // Zbrajamo koeficijente clanova s istim eksponentom

        free(newNode);  // Oslobadamo nepotrebno alocirani cvor

        if (temp->next->coeff == 0) {
            Position toDelete = temp->next;
            // Spremamo cvor koji treba obrisati

            temp->next = toDelete->next;
            // Prespajamo listu kako bismo izbacili taj cvor

            free(toDelete);  // Oslobadamo memoriju
        }
    }
    else {
        newNode->next = temp->next;
        temp->next = newNode;
        // Ubacujemo novi cvor na odgovarajuce mjesto
    }

    return 0;
}


// Ucitavanje polinoma iz datoteke
int ReadFromFile(Position head, const char* filename) {

    FILE* fp = fopen(filename, "r");
    // Otvaramo datoteku za citanje

    if (fp == NULL) {
        printf("Datoteku nije moguce otvoriti!\n");
        return -1;
    }

    int coeff, pow;
    int status;

    while ((status = fscanf(fp, "%d %d", &coeff, &pow)) == 2) {
        // Citamo koeficijent i eksponent iz datoteke

        if (InsertSorted(head, coeff, pow) != 0) {
            // Svaki procitani clan ubacujemo sortirano

            fclose(fp);  // Zatvaramo datoteku
            return -1;
        }
    }

    if (!feof(fp)) {
        fclose(fp);
        printf("Pogresan format datoteke!\n");
        return -1;
    }

    fclose(fp);  // Zatvaramo datoteku nakon citanja
    return 0;
}


// Zbrajanje dva polinoma
int AddPolynomials(Position result, Position p1, Position p2) {

    Position temp;

    for (temp = p1->next; temp != NULL; temp = temp->next) {
        // Prolazimo kroz prvi polinom

        if (InsertSorted(result, temp->coeff, temp->pow) != 0)
            return -1;  // Provjera uspjesnog umetanja
    }

    for (temp = p2->next; temp != NULL; temp = temp->next) {
        // Prolazimo kroz drugi polinom

        if (InsertSorted(result, temp->coeff, temp->pow) != 0)
            return -1;
    }

    return 0;
}


// Mnozenje dva polinoma
int MultiplyPolynomials(Position result, Position p1, Position p2) {

    Position temp1, temp2;

    for (temp1 = p1->next; temp1 != NULL; temp1 = temp1->next) {
        // Prolazimo kroz prvi polinom

        for (temp2 = p2->next; temp2 != NULL; temp2 = temp2->next) {
            // Svaki clan prvog mnozimo sa svim clanovima drugog

            int newCoeff = temp1->coeff * temp2->coeff;
            // Mnozimo koeficijente

            int newPow = temp1->pow + temp2->pow;
            // Zbrajamo eksponente

            if (InsertSorted(result, newCoeff, newPow) != 0)
                return -1;  // Dodajemo dobiveni clan u rezultat
        }
    }

    return 0;
}


// Ispis polinoma
int Print(Position head) {

    Position temp = head->next;
    // Pokazivac postavljamo na prvi clan polinoma

    if (temp == NULL) {
        printf("0\n");
        return 0;  // Ispisujemo nulu ako je polinom prazan
    }

    while (temp != NULL) {
        // Prolazimo kroz sve clanove polinoma

        if (temp != head->next && temp->coeff > 0)
            printf("+");  // Ispisujemo plus ispred pozitivnog clana

        if (temp->coeff < 0)
            printf("-");  // Ispisujemo minus ispred negativnog clana

        int absCoeff = abs(temp->coeff);
        // Uzimamo apsolutnu vrijednost koeficijenta

        if (temp->pow == 0)
            printf("%d", absCoeff);  // Ispis konstante

        else if (temp->pow == 1)
            printf("%dx", absCoeff);  // Ispis clana s eksponentom 1

        else
            printf("%dx^%d", absCoeff, temp->pow);
        // Ispis clana s ostalim eksponentima

        temp = temp->next;  // Prelazimo na sljedeci clan
    }

    printf("\n");
    return 0;
}


// Oslobadanje memorije
void DeleteList(Position head) {

    Position temp;

    while (head->next != NULL) {
        temp = head->next;
        // Spremamo prvi clan liste

        head->next = temp->next;
        // Pomicemo pocetak liste na sljedeci clan

        free(temp);  // Brisemo prethodni clan iz memorije
    }
}


// Glavni program
int main() {

    // Inicijalizacija zaglavnih cvorova
    Poly first = { 0, 0, NULL };
    Poly second = { 0, 0, NULL };
    Poly sum = { 0, 0, NULL };
    Poly product = { 0, 0, NULL };

    if (ReadFromFile(&first, "prvipolinom.txt") != 0) {
        // Ucitavamo prvi polinom i provjeravamo gresku

        printf("Greska pri ucitavanju prvog polinoma!\n");
        goto cleanup;
    }

    if (ReadFromFile(&second, "drugipolinom.txt") != 0) {
        // Ucitavamo drugi polinom

        printf("Greska pri ucitavanju drugog polinoma!\n");
        goto cleanup;
    }

    printf("Prvi polinom: ");
    Print(&first);

    printf("Drugi polinom: ");
    Print(&second);

    if (AddPolynomials(&sum, &first, &second) != 0) {
        // Pozivamo funkciju za zbrajanje

        printf("Greska pri zbrajanju!\n");
        goto cleanup;
    }

    printf("Zbroj polinoma: ");
    Print(&sum);

    if (MultiplyPolynomials(&product, &first, &second) != 0) {
        // Pozivamo funkciju za mnozenje

        printf("Greska pri mnozenju!\n");
        goto cleanup;
    }

    printf("Umnozak polinoma: ");
    Print(&product);

cleanup:

    // Oslobadamo memoriju svih polinoma
    DeleteList(&first);
    DeleteList(&second);
    DeleteList(&sum);
    DeleteList(&product);

    return 0;
}
