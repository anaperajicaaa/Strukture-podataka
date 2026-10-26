#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h> // Ukljucuje funkcije za ispis i rad s datotekama
#include <stdlib.h> // Ukljucuje funkcije malloc, realloc i free za rad s memorijom
#include <string.h> // Ukljucuje funkciju strcmp za usporedivanje prezimena

typedef struct _Student { // Definira strukturu koja sadrzi podatke o studentu
    char name[30]; // Sprema ime studenta
    char lastname[30]; // Sprema prezime studenta
    int points; // Sprema broj bodova studenta
} Student; // Omogucuje da strukturu koristimo pod imenom Student

/* A. Dodavanje elementa iza odredenog elementa */
Student* dodajIza(Student* students, int* n, int pozicija, Student novi) { // Prima listu, broj studenata, poziciju i novog studenta
    if (pozicija < 0 || pozicija >= *n) // Provjerava je li pozicija unutar granica liste
        return students; // Ako nije, vraca nepromijenjenu listu

    Student* temp = realloc(students, (*n + 1) * sizeof(Student)); // Povecava memoriju kako bi stao jos jedan student
    if (temp == NULL) // Provjerava je li prosirenje memorije uspjelo
        return students; // Ako nije, vraca postojecu listu

    students = temp; // Sprema novu adresu liste nakon prosirenja memorije

    for (int i = *n; i > pozicija + 1; i--) // Pomice elemente od kraja prema odabranoj poziciji
        students[i] = students[i - 1]; // Pomice svakog studenta jedno mjesto udesno

    students[pozicija + 1] = novi; // Postavlja novog studenta iza odabranog studenta
    (*n)++; // Povecava broj studenata za jedan

    return students; // Vraca adresu liste jer se adresa nakon realloc moze promijeniti
} // Zavrsava funkciju dodavanja iza

/* B. Dodavanje elementa ispred odredenog elementa */
Student* dodajIspred(Student* students, int* n, int pozicija, Student novi) { // Prima listu, broj studenata, poziciju i novog studenta
    if (pozicija < 0 || pozicija >= *n) // Provjerava je li pozicija valjana
        return students; // Ako nije, ne mijenja listu

    Student* temp = realloc(students, (*n + 1) * sizeof(Student)); // Prosiruje memoriju za jednog dodatnog studenta
    if (temp == NULL) // Provjerava je li prosirenje memorije uspjelo
        return students; // Ako nije, vraca postojecu listu

    students = temp; // Sprema novu adresu prosirene liste

    for (int i = *n; i > pozicija; i--) // Pomice elemente od kraja prema odabranoj poziciji
        students[i] = students[i - 1]; // Pomice studente jedno mjesto udesno kako bi napravio prostor

    students[pozicija] = novi; // Smjesta novog studenta ispred odabranog studenta
    (*n)++; // Povecava broj studenata za jedan

    return students; // Vraca adresu liste
} // Zavrsava funkciju dodavanja ispred

/* C. Sortiranje po prezimenima */
void sortirajPoPrezimenima(Student* students, int n) { // Prima listu i broj studenata
    for (int i = 0; i < n - 1; i++) { // Odabire studenta kojeg usporedujemo s ostalima
        for (int j = i + 1; j < n; j++) { // Prolazi kroz studente koji dolaze nakon odabranog
            if (strcmp(students[i].lastname, students[j].lastname) > 0) { // Provjerava je li prvo prezime abecedno iza drugog
                Student temp = students[i]; // Privremeno sprema prvog studenta
                students[i] = students[j]; // Na njegovo mjesto stavlja drugog studenta
                students[j] = temp; // Na drugo mjesto stavlja prethodno spremljenog studenta
            } 
        } 
    } 
} // Zavrsava funkcija sortiranja

/* D. Upis liste u datoteku */
void upisiUDatoteku(Student* students, int n, const char* naziv) { // Prima listu, broj studenata i naziv datoteke
    FILE* fp = fopen(naziv, "w"); // Otvara datoteku za pisanje; prethodni sadrzaj se brise

    if (fp == NULL) { // Provjerava je li datoteku moguce otvoriti
        printf("Greska pri otvaranju datoteke!\n"); // Ispisuje poruku o gresci
        return; // Prekida funkciju ako datoteku nije moguce otvoriti
    } // Zavrsava provjeru otvaranja datoteke

    for (int i = 0; i < n; i++) { // Prolazi kroz sve studente
        fprintf(fp, "%s %s %d\n", // Zapisuje ime, prezime i bodove u datoteku
            students[i].name, // Predaje ime studenta za zapisivanje
            students[i].lastname, // Predaje prezime studenta za zapisivanje
            students[i].points); // Predaje broj bodova za zapisivanje
    } // Zavrsava zapisivanje svih studenata

    fclose(fp); // Zatvara datoteku nakon zapisivanja
} // Zavrsava funkcija upisa

/* E. Citanje liste iz datoteke */
Student* citajIzDatoteke(const char* naziv, int* n) { // Prima naziv datoteke i adresu varijable za broj studenata
    FILE* fp = fopen(naziv, "r"); // Otvara datoteku za citanje

    if (fp == NULL) { // Provjerava je li datoteku moguce otvoriti
        printf("Greska pri otvaranju datoteke!\n"); // Ispisuje poruku ako otvaranje nije uspjelo
        *n = 0; // Postavlja broj studenata na nulu
        return NULL; // Vraca NULL jer citanje nije uspjelo
    } // Zavrsava provjeru otvaranja datoteke

    int kapacitet = 2; // Na pocetku predvida memoriju za dva studenta
    *n = 0; // Postavlja broj procitanih studenata na nulu

    Student* students = malloc(kapacitet * sizeof(Student)); // Zauzima memoriju za dva studenta

    if (students == NULL) { // Provjerava je li memorija uspjesno zauzeta
        fclose(fp); // Zatvara datoteku jer citanje ne moze nastaviti
        return NULL; // Vraca NULL ako nema dostupne memorije
    } // Zavrsava provjeru memorije

    Student novi; // Stvara privremenu varijablu za podatke jednog studenta

    while (fscanf(fp, "%29s %29s %d", // Pokusava procitati ime, prezime i broj bodova
        novi.name, novi.lastname, &novi.points) == 3) { // Ponavlja citanje dok uspjesno procita sva tri podatka

        if (*n == kapacitet) { // Provjerava je li popunjen cijeli kapacitet polja
            kapacitet *= 2; // Udvostrucuje kapacitet polja

            Student* temp = realloc(students, kapacitet * sizeof(Student)); // Prosiruje memoriju za novi kapacitet

            if (temp == NULL) { // Provjerava je li prosirenje memorije uspjelo
                free(students); // Oslobada prethodno zauzetu memoriju
                fclose(fp); // Zatvara otvorenu datoteku
                *n = 0; // Postavlja broj studenata na nulu
                return NULL; // Prekida citanje jer nema dovoljno memorije
            } // Zavrsava provjeru prosirenja memorije

            students = temp; // Sprema novu adresu prosirenog polja
        } // Zavrsava provjeru kapaciteta

        students[*n] = novi; // Sprema procitanog studenta na prvo slobodno mjesto
        (*n)++; // Povecava broj procitanih studenata za jedan
    } // Zavrsava petlju kada vise nema potpunih zapisa za citanje

    fclose(fp); // Zatvara datoteku nakon citanja

    if (*n == 0) { // Provjerava je li procitan barem jedan student
        free(students); // Oslobada memoriju jer nema podataka
        return NULL; // Vraca NULL ako je lista prazna
    } // Zavrsava provjeru prazne liste

    return students; // Vraca adresu polja s procitanim studentima
} // Zavrsava funkciju citanja

int main(void) { // Glavna funkcija u kojoj zapocinje program
    int n = 0; // Stvara varijablu za broj studenata i postavlja je na nulu

    Student* students = citajIzDatoteke("data.txt", &n); // Ucitava studente iz datoteke i sprema njihov broj u n

    if (students == NULL) { // Provjerava je li lista uspjesno ucitana i nije prazna
        printf("Lista nije ucitana ili je prazna.\n"); // Ispisuje poruku ako nema ucitanih studenata
        return 1; // Zavrsava program zbog greske
    } // Zavrsava provjeru ucitavanja

    /* A. Dodavanje iza prvog studenta */
    Student novi1 = { "Marko", "Horvat", 80 }; // Stvara novog studenta Marka Horvata s 80 bodova
    Student* temp = dodajIza(students, &n, 0, novi1); // Dodaje Marka iza studenta na poziciji 0, odnosno prvog studenta

    if (temp != NULL) // Provjerava je li vracen valjan pokazivac
        students = temp; // Sprema adresu liste nakon moguce promjene memorije

    /* B. Dodavanje ispred drugog studenta */
    Student novi2 = { "Ana", "Kovac", 90 }; // Stvara novu studenticu Anu Kovac s 90 bodova
    temp = dodajIspred(students, &n, 1, novi2); // Dodaje Anu ispred studenta koji je sada na poziciji 1

    if (temp != NULL) // Provjerava je li vracen valjan pokazivac
        students = temp; // Sprema adresu liste nakon moguce promjene memorije

    /* C. Sortiranje po prezimenima */
    sortirajPoPrezimenima(students, n); // Sortira studente abecedno po prezimenima

    /* D. Upis sortirane liste u datoteku */
    upisiUDatoteku(students, n, "data.txt"); // Sprema sortiranu listu u datoteku data.txt

    /* Ispis liste */
    printf("Lista studenata:\n"); // Ispisuje naslov liste

    for (int i = 0; i < n; i++) { // Prolazi kroz sve studente u polju
        printf("%s %s - %d\n", // Ispisuje ime, prezime i bodove jednog studenta
            students[i].name, // Predaje ime za ispis
            students[i].lastname, // Predaje prezime za ispis
            students[i].points); // Predaje bodove za ispis
    } // Zavrsava ispis svih studenata

    free(students); // Oslobada memoriju koju je koristila lista studenata

    /* E. Ponovno citanje iz datoteke */
    students = citajIzDatoteke("data.txt", &n); // Ponovno ucitava spremljenu listu iz datoteke

    if (students != NULL) { // Provjerava je li ponovno citanje uspjelo
        printf("\nLista procitana iz datoteke:\n"); // Ispisuje naslov za ponovno ucitanu listu

        for (int i = 0; i < n; i++) { // Prolazi kroz sve ponovno ucitane studente
            printf("%s %s - %d\n", // Ispisuje podatke svakog studenta
                students[i].name, // Predaje ime za ispis
                students[i].lastname, // Predaje prezime za ispis
                students[i].points); // Predaje bodove za ispis
        } // Zavrsava ispis ponovno ucitanih studenata

        free(students); // Oslobada memoriju nakon zavrsetka ispisa
    } // Zavrsava provjeru uspjesnog ponovnog citanja

    return 0; 
}
