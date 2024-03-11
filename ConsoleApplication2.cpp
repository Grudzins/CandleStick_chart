//Jakub_Grudziński_197973
#include <iostream>
#include <iomanip>
#include <ctime>

using namespace std;

#define max_dlugosc_daty 20
#define max_dlugosc_pliku 50
#define max_dlugosc_log 100
#pragma warning(disable:4996)

struct WartosciGieldowe {
    char date[max_dlugosc_daty];
    double open;
    double high;
    double low;
    double close;
    double volume;
};

int ustalona_wysokosc_wykresu = 50;
int ustalony_rozmiar_wykresu = 200;

void generujWykresSwiecowy(WartosciGieldowe dane[], const char* plikWyjsciowy, int startIndex, int endIndex, int szerokoscSwiecy, int wysokoscWykresu) {
    const char cialo_bialej_swiecy = 'O';
    const char cialo_czarnej_swiecy = '#';
    const char cien_swiecy = '|';
    const char spacja = ' ';

    FILE* plik = fopen(plikWyjsciowy, "w");
    if (plik == nullptr) {
        cout << "Nie mozna otworzyc pliku do zapisu.\n";
        return;
    }

    int zakresDanych = endIndex - startIndex + 1;
    int startIndexZakres = (zakresDanych > szerokoscSwiecy) ? zakresDanych - szerokoscSwiecy : 0;

    double maxCena = 0;
    double minCena = 100000;

    for (int i = startIndexZakres; i < zakresDanych; ++i) {
        maxCena = (maxCena > dane[startIndex + i].high) ? maxCena : dane[startIndex + i].high;
        minCena = (minCena < dane[startIndex + i].low) ? minCena : dane[startIndex + i].low;
    }

    double zakresCeny = maxCena - minCena;
    double schodekCenowy = zakresCeny / wysokoscWykresu;

    for (double cena = maxCena; cena >= minCena; cena -= schodekCenowy) {
        fprintf(plik, "%10.2f |", cena);
        for (int i = startIndex; i <= endIndex; ++i) {
            if (cena >= dane[i].low && cena <= dane[i].high) {
                if (dane[i].open < dane[i].close) {
                    if (cena >= dane[i].open && cena <= dane[i].close) {
                        fprintf(plik, "%c", cialo_bialej_swiecy);
                    }
                    else if (cena >= dane[i].low && cena <= dane[i].open) {
                        fprintf(plik, "%c", cien_swiecy);
                    }
                    else if (cena >= dane[i].close && cena <= dane[i].high) {
                        fprintf(plik, "%c", cien_swiecy);
                    }
                    else {
                        fprintf(plik, "%c", spacja);
                    }
                }
                else {
                    if (cena >= dane[i].close && cena <= dane[i].open) {
                        fprintf(plik, "%c", cialo_czarnej_swiecy);
                    }
                    else if (cena >= dane[i].low && cena <= dane[i].close) {
                        fprintf(plik, "%c", cien_swiecy);
                    }
                    else if (cena >= dane[i].open && cena <= dane[i].high) {
                        fprintf(plik, "%c", cien_swiecy);
                    }
                    else {
                        fprintf(plik, "%c", spacja);
                    }
                }
            }
            else {
                fprintf(plik, "%c", spacja);
            }
        }
        fprintf(plik, "\n");
    }

    fclose(plik);
    cout << "Wygenerowano wykres swiecowy w pliku '" << plikWyjsciowy << "'.\n";
}

void zmienWysokoscWykresu(int& wysokoscWykresu) {
    cout << "Aktualna wysokosc wykresu: " << wysokoscWykresu << endl;
    cout << "Podaj nowa wysokosc wykresu: ";
    int nowaWysokosc;
    cin >> nowaWysokosc;
    wysokoscWykresu = nowaWysokosc;

}

void zmienZakresDanych(int& startIndex, int& endIndex, int rozmiarDanych) {
    cout << "Aktualny indeks poczatkowy: " << startIndex << endl;
    cout << "Podaj nowy indeks poczatkowy (0-" << rozmiarDanych - 1 << "): ";
    cin >> startIndex;

    cout << "Aktualny indeks koncowy: " << endIndex << endl;
    cout << "Podaj nowy indeks koncowy (0-" << rozmiarDanych - 1 << "): ";
    cin >> endIndex;


    startIndex = (startIndex < 0) ? 0 : (startIndex >= rozmiarDanych) ? rozmiarDanych - 1 : startIndex;
    endIndex = (endIndex < 0) ? 0 : (endIndex >= rozmiarDanych) ? rozmiarDanych - 1 : endIndex;
}

void zmienSzerokoscSwiec(int& szerokoscSwiecy) {
    cout << "Wybierz szerokosc jednej swiecy:\n";
    cout << "1. Jeden tydzien\n";
    cout << "2. Jeden miesiac\n";
    cout << "Wybierz opcje: ";
    int wybor;
    cin >> wybor;

    switch (wybor) {
    case 1:
        szerokoscSwiecy = 5;
        break;
    case 2:
        szerokoscSwiecy = 20;
        break;
    default:
        cout << "Niepoprawny wybor. Ustawiono domyslna wartosc (tydzien).\n";
        szerokoscSwiecy = 5;
        break;
    }
}

int czytajCSV(const char* nazwaPliku, WartosciGieldowe*& dane) {
    FILE* plik = fopen(nazwaPliku, "r");
    if (plik == nullptr) {
        cout << "Nie mozna otworzyc pliku!\n";
        return 0;
    }

    int rozmiarDanych = 0;
    const int ustalonyRozmiarDanych = 200;
    char linia[max_dlugosc_log];
    fgets(linia, max_dlugosc_log, plik);

    while (fgets(linia, max_dlugosc_log, plik) != nullptr) {
        rozmiarDanych++;
    }

    fseek(plik, 0, SEEK_SET);
    fgets(linia, max_dlugosc_log, plik);

    rozmiarDanych = (rozmiarDanych > ustalonyRozmiarDanych) ? ustalonyRozmiarDanych : rozmiarDanych;

    dane = new WartosciGieldowe[rozmiarDanych];
    int index = 0;

    while (fgets(linia, max_dlugosc_log, plik) != nullptr && index < rozmiarDanych) {
        sscanf(linia, "%19[^,],%lf,%lf,%lf,%lf,%lf", dane[index].date, &dane[index].open, &dane[index].high, &dane[index].low, &dane[index].close, &dane[index].volume);
        index++;
    }

    fclose(plik);
    return rozmiarDanych;
}

void wyswietlCzescWykresu(WartosciGieldowe dane[], int startIdx, int endIdx, int wysokoscWykresu) {
    const char cialo_bialej_swiecy = 'O';
    const char cialo_czarnej_swiecy = '#';
    const char cien_swiecy = '|';
    const char spacja = ' ';

    double maxCena = 0;

    double minCena = 100000;

    for (int i = startIdx; i <= endIdx; ++i) {
        maxCena = (dane[i].high > maxCena) ? dane[i].high : maxCena;
        minCena = (dane[i].low < minCena) ? dane[i].low : minCena;
    }

    double zakresCeny = maxCena - minCena;
    double schodekCenowy = zakresCeny / wysokoscWykresu;

    if (endIdx - startIdx > wysokoscWykresu) {
        schodekCenowy = zakresCeny / (endIdx - startIdx);
    }

    for (double cena = maxCena; cena >= minCena; cena -= schodekCenowy) {
        cout << setw(10) << cena << " | ";

        for (int i = startIdx; i <= endIdx; ++i) {
            if (cena >= dane[i].low && cena <= dane[i].high) {
                if (dane[i].open < dane[i].close) {
                    if (cena >= dane[i].open && cena <= dane[i].close) {
                        cout << cialo_bialej_swiecy;
                    }
                    else if (cena >= dane[i].low && cena <= dane[i].open) {
                        cout << cien_swiecy;
                    }
                    else if (cena >= dane[i].close && cena <= dane[i].high) {
                        cout << cien_swiecy;
                    }
                    else {
                        cout << spacja;
                    }
                }
                else {
                    if (cena >= dane[i].close && cena <= dane[i].open) {
                        cout << cialo_czarnej_swiecy;
                    }
                    else if (cena >= dane[i].low && cena <= dane[i].close) {
                        cout << cien_swiecy;
                    }
                    else if (cena >= dane[i].open && cena <= dane[i].high) {
                        cout << cien_swiecy;
                    }
                    else {
                        cout << spacja;
                    }
                }
            }
            else {
                cout << spacja;
            }
        }
        cout << endl;
    }
}

void wybierzPlikWyjsciowy(char* nazwaPliku) {
    cout << "Podaj nazwe pliku wyjsciowego (razem z rozszerzeniem .txt): ";
    cin >> nazwaPliku;
}

void wybierzPlikWejsciowy(char* nazwaPliku) {
    cout << "Podaj nazwe pliku wejsciowego (razem z rozszerzeniem .csv): ";
    cin >> nazwaPliku;
}

void wyswietlMenu() {
    cout << "==== MENU ====\n";
    cout << "g -  Wygeneruj wykres swiecowy\n";
    cout << "q - Wyjdz z programu\n";
    cout << "1. Wybierz plik wejsciowy\n";
    cout << "2. Wybierz plik wyjsciowy\n";
    cout << "3. Zmien wysokosc wykresu\n";
    cout << "4. Zmien szerokosc wykresu\n";
    cout << "5. Wyswietl kawalek wykresu(0-50)\n";
    cout << "Wybierz opcje: ";
}

void wyswietlPrzywitanie() {
    cout << "Witaj! Autorem projektu jest: Jakub Grudzinski 197973\n";
}

void zapiszAkcjeUzytkownika(const char* akcja) {
    FILE* plikLog = fopen("akcje_uzytkownika.log", "a");
    if (plikLog != nullptr) {
        time_t aktualnyCzas;
        time(&aktualnyCzas);
        fprintf(plikLog, "%s: %s\n", ctime(&aktualnyCzas), akcja);
        fclose(plikLog);
    }
    else {
        cout << "Nie mozna otworzyc pliku logu." << endl;
    }
}

int main() {
    char nazwaPliku[max_dlugosc_pliku];
    wyswietlPrzywitanie();
    WartosciGieldowe* wartosci = nullptr;
    int rozmiarDanych = czytajCSV("intc_us_data.csv", wartosci);
    char plikWyjsciowy[max_dlugosc_pliku] = "chart.txt";

    if (rozmiarDanych == 0) {
        return 1;
    }
    int startIndex = 0;
    int endIndex = rozmiarDanych - 1;
    bool startIndexAktualizowany = false;
    bool endIndexAktualizowany = false;
    char wybor = '0';
    int zakres = 0;
    do {

        wyswietlMenu();
        cin >> wybor;

        switch (wybor) {
        case 'g': {
            if (startIndexAktualizowany || endIndexAktualizowany) {
                generujWykresSwiecowy(wartosci, plikWyjsciowy, startIndex, endIndex, ustalony_rozmiar_wykresu, ustalona_wysokosc_wykresu);
                zapiszAkcjeUzytkownika("Wybrano opcje : Generuj wykres");
            }
            else {
                generujWykresSwiecowy(wartosci, plikWyjsciowy, startIndex, rozmiarDanych - 1, ustalony_rozmiar_wykresu, ustalona_wysokosc_wykresu);
                zapiszAkcjeUzytkownika("Wybrano opcje : Generuj wykres");
            }
            break;
        }
        case 'q':
            cout << "Zamykanie programu..." << endl;
            zapiszAkcjeUzytkownika("Wybrano opcje : Zakoncz program");
            break;
        case '1':
            wybierzPlikWejsciowy(nazwaPliku);
            rozmiarDanych = czytajCSV(nazwaPliku, wartosci);
            zapiszAkcjeUzytkownika("Wybrano opcje : Wybierz plik wejsciowy");
            break;
        case '2':
            wybierzPlikWyjsciowy(plikWyjsciowy);
            zapiszAkcjeUzytkownika("Wybrano opcje : Wybierz plik wyjsciowy");
            break;
        case '3':
            zmienWysokoscWykresu(ustalona_wysokosc_wykresu);
            generujWykresSwiecowy(wartosci, plikWyjsciowy, startIndex, endIndex, ustalony_rozmiar_wykresu, ustalona_wysokosc_wykresu);
            zapiszAkcjeUzytkownika("Wybrano opcje : Zmien wysokosc wykresu");
            break;
        case '4':
            cout << "Wybierz zakres: " << endl;
            cout << "1 - wedlug tygodni, miesiecy " << endl;
            cout << "2 - dowolny zakres: " << endl;
            cin >> zakres;
            switch (zakres) {
            case 1:
                zmienSzerokoscSwiec(ustalony_rozmiar_wykresu);
                generujWykresSwiecowy(wartosci, plikWyjsciowy, startIndex, endIndex, ustalony_rozmiar_wykresu, ustalona_wysokosc_wykresu);
                zapiszAkcjeUzytkownika("Wybrano opcje : Zmien szerokosc wykresu");
                break;
            case 2:
                zmienZakresDanych(startIndex, endIndex, rozmiarDanych);
                startIndexAktualizowany = true;
                endIndexAktualizowany = true;
                zapiszAkcjeUzytkownika("Wybrano opcje : Zmien poczatek i koniec zakresu dat");
                break;
            default:
                cout << "Niewlasciwy wybor!";
                break;
            }
            break;
        case '5':
            wyswietlCzescWykresu(wartosci, 0, 50, ustalona_wysokosc_wykresu);
            zapiszAkcjeUzytkownika("Wybrano opcje : Wyswietl kawalek wykresu");
            break;

        default:
            cout << "Niepoprawny wybor. Sprobuj ponownie." << endl;
            zapiszAkcjeUzytkownika("Wybrano opcje : Niepoprawny wybor");
            break;
        }

    } while (wybor != 'q');

    delete[] wartosci;
    return 0;
}


