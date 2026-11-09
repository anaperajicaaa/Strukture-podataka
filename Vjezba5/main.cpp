#include <iostream>
#include <fstream>   // Rad s datotekama
#include <string>
#include <sstream>   // Čitanje znakova iz izraza
#include <stdexcept>
using namespace std;

// Čvor vezane liste
struct Cvor {
    double podatak;
    Cvor* sljedeci; // Pokazivač na sljedeći čvor
};

// Klasa Stog
class Stog {
private:
    Cvor* vrh; // Pokazivač na vrh stoga

public:
    Stog() {
        vrh = nullptr; // Stog je na početku prazan
    }

    // Dodavanje elementa na vrh stoga
    void push(double x) {
        Cvor* novi = new Cvor; // Stvaranje novog čvora
        novi->podatak = x;
        novi->sljedeci = vrh; // Povezivanje s prethodnim vrhom
        vrh = novi; // Novi čvor postaje vrh
    }

    // Uklanjanje elementa s vrha stoga
    double pop() {
        if (vrh == nullptr) {
            throw runtime_error("Stog je prazan!");
        }

        Cvor* pomocni = vrh;
        double x = vrh->podatak;

        vrh = vrh->sljedeci; // Pomicanje vrha na sljedeći čvor
        delete pomocni; // Brisanje starog vrha

        return x; // Vraćanje uklonjenog podatka
    }

    // Provjera je li stog prazan
    bool prazan() {
        return vrh == nullptr;
    }

    // Oslobađanje memorije
    ~Stog() {
        while (!prazan()) {
            pop();
        }
    }
};

// Funkcija za računanje postfiksnog izraza
double izracunaj(string izraz) {
    Stog s; // Stvaranje stoga
    stringstream ss(izraz);
    string znak;

    // Čitanje izraza znak po znak
    while (ss >> znak) {

        // Provjera je li znak operator
        if (znak == "+" || znak == "-" ||
            znak == "*" || znak == "/") {

            double b = s.pop(); // Uzimanje drugog operanda
            double a = s.pop(); // Uzimanje prvog operanda

            // Izvođenje odgovarajuće operacije
            if (znak == "+")
                s.push(a + b);
            else if (znak == "-")
                s.push(a - b);
            else if (znak == "*")
                s.push(a * b);
            else if (znak == "/") {
                if (b == 0)
                    throw runtime_error("Dijeljenje s nulom!");
                s.push(a / b);
            }
        }
        else {
            s.push(stod(znak)); // Broj se dodaje na stog
        }
    }

    double rezultat = s.pop(); // Uzimanje konačnog rezultata

    if (!s.prazan()) {
        throw runtime_error("Neispravan postfiksni izraz!");
    }

    return rezultat;
}

int main() {
    // Otvaranje datoteke
    ifstream datoteka("izraz.txt");

    if (!datoteka) {
        cout << "Greska pri otvaranju datoteke!" << endl;
        return 1;
    }

    string izraz;
    getline(datoteka, izraz); // Čitanje izraza iz datoteke
    datoteka.close(); // Zatvaranje datoteke

    try {
        double rezultat = izracunaj(izraz);
        cout << "Rezultat izraza je: " << rezultat << endl;
    }
    catch (exception& e) {
        cout << "Greska: " << e.what() << endl;
    }

    return 0;
}
