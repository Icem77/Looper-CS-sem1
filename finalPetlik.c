/***************************************************************************************************
    AUTOR: SZYMON KRUK

    OPIS:
    Program jest realizacja języka Petlik.
    Program czyta i wykonuje polecenia:
    - wypisania wartości zmiennej;
    - wykonania programu w języku Petlik.

    Przed wykonaniem pierwszego polecenia wartosci wszystkich zmiennych są rowne 0.
    Zmienne zachowują wartosci po wykonaniu polecenia. Nie sa zerowane przed kazdym wykonaniem
    programu.

    Składnie jezyka opisuje gramatyka z symbolem poczatkowym Program:
    Program → CiagInstrukcji
    CiagInstrukcji → ε | CiagInstrukcji Instrukcja
    Instrukcja → Zwieksz | Powtarzaj
    Zwieksz → Zmienna
    Powtarzaj → '(' Zmienna CiągInstrukcji ')'
    Zmienna → 'a' | .. | 'z'

    Program sklada się ze znakow, ktore w gramatyce są ujete w apostrofy. Oprocz nich zadne inne
    znaki, nawet spacje lub konce wiersza, w kodzie zrodlowym nie moga wystąpic. Program ma dostep
    do 26 zmiennych, ktorych wartosci są nieujemnymi liczbami calkowitymi.

    Instrukcje wyprowadzone z symbolu CiagInstrukcji są wykonywane w kolejnosci od pierwszej do
    ostatniej. Instrukcja Zwieksz, w postaci zmienna, jest rownowazna instrukcji jezyka 'C':
    ++zmienna;
    Instrukcja Powtarzaj, w postaci (zmienna...), jest rownowazna instrukcji jezyka 'C':
    while (zmienna > 0) {
        --zmienna;
        ...
    }

    Jezeli w instrukcji Powtarzaj nie jest zagniezdzona inna instrukcja Powtarzaj, czyli
    w nawiasach jest ciag zmiennych Zmienna0, ..., ZmiennaN, dla N >= 0, i jezeli żadna ze zmiennych
    Zmienna1, ..., ZmiennaN nie jest zmienna Zmienna0, to kompilator generuje kod zoptymalizowany
    polegajacy na dodaniu do zmiennych Zmienna(1-N) wartosci zmiennej Zmienna0 i wyzerowaniu
    jej wartosci.

    Jesli nie sa spełnione warunki umozliwiajace wygenerowanie dla instrukcji kodu
    zoptymalizowanego, to kompilator generuje kod zwykly.

    WEJSCIE:
    Dane programu to ciag wierszy, w każdym po jednym poleceniu.
    Polecenie wypisania wartosci zmiennej zaczyna się od znaku '=', po ktorym jest nazwa zmiennej.
    Wiersz polecenia wykonania programu zawiera kod zrodlowy w jezyku Petlik.
    Kazdy wiersz wejscia konczy sie znakiem nowej linii.

    WYJSCIE:
    Wynik programu jest efektem wykonania polecen wypisania wartosci zmiennej.
    Wartosc zmiennej piszemy dziesietnie, w jednym wierszu, bez nieznaczacych zer wiodacych.
    Wartosc 0 jest reprezentowana przez cyfre 0 a wartosci dodatnie zaczynaja sie od cyfry
    niezerowej.
    Kazdy wiersz wyjscia konczy sie znakiem nowej linii.

    KONWENCJA:
    Wartosci zmiennych przechowywane sa w tablicach, ktorych adresy reprezentuje tablica 'wartosci'
    o rozmiarze 26. Dlugosci zmiennych przechowywane sa w tablicy 'dlugosci' o rozmiarze 26.
    Danym zmiennej 'a' odpowiada indeks '0', zmiennej 'b' indeks '1' ... zmiennej 'z' indeks '25'.
    Program wielokrotnie wykorzystuje fakt ze indeks ten latwo obliczyc odejmujac liczbe '97' od
    reprezentacji zmiennej w systemi ASCII.

    Wartosci zmiennych sa zapisane w tablicach od cyfr najmniej do najbardziej znaczacych.
    Przez dlugosc zmiennej rozumiemy ilosc cyfr tworzacych jej wartosc np. '0' ma dlugosc '1',
    '900' ma dlugosc '3' itd.

    ZALOZENIA:
    - Podczas pracy programu nie nastapi przepelnienie pamieci.
    - Dlugosc programu w języku Petlik nie przekroczy INT_MAX - 1.
    - Wartosci zmiennych, zapisane dziesietnie, beda mialy mniej niż INT_MAX cyfr.
***************************************************************************************************/

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

/**
    a - liczba calkowita

    b - liczba calkowita

    Zwraca wieksza sposrod liczb 'a' i 'b'.
    Jezeli liczby 'a' i 'b' sa rowne zwraca wartosc 'a'.
*/
int max(int a, int b)
{
    if (a >= b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

/**
    wartosci - tablica o rozmiarze 26 zawierajaca adresy tablic
    przechowujacych wartosci zmiennych

    dlugosci - tablica o rozmiarze 26 zawierajaca dlugosci zmiennych

    Inicjalizuje tablice o adresach z tablicy 'wartosci' zerami w ilosci 'INT_MAX'
    oraz tablice 'dlugosci' wartoscia 1.

    UWAGA: Funkcja alokuje pamiec ktora nalezy pozniej samodzielnie zwolnic!
*/
void inicjalizuj_zmienne(int *wartosci[26], int dlugosci[])
{
    for (int i = 0; i < 26; i++)
    {
        wartosci[i] = (int *) calloc(1000, sizeof(int));
        dlugosci[i] = 1;
    }
}

/**
    wartosci - tablica o rozmiarze 26 zawierajaca adresy tablic
    przechowujacych wartosci zmiennych

    Zwalnia pamiec pod adresami przechowywanymi w tablicy 'wartosci'.
*/
void zwolnij_pamiec_zmiennych(int *wartosci[26])
{
    for (int i = 0; i < 26; i++)
    {
        free(wartosci[i]);
    }
}

/**
    wartosc - wskaznik do tablicy zawierajacej dziesietny zapis zmiennej,
    od najmniej do najbardziej znaczacych cyfr

    dlugosc - ilosc cyfr tworzacych wartosc zmiennej

    Zwraca '1' jezeli wartosc liczby w tablicy o adresie 'wartosc' jest zerem.
    W przeciwnym wypadku zwraca '0'.
*/
int jest_zerem(int *wartosc, int dlugosc)
{
    if (wartosc[0] == 0 && dlugosc == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
    wartosc - wskaznik do tablicy zawierajacej dziesietny zapis zmiennej,
    od najmniej do najbardziej znaczacych cyfr

    dlugosc - ilosc cyfr tworzacych wartosc zmiennej

    Wypisuje do standardowego wyjscia wartosc zmiennej reprezentowanej przez tablice
    na ktora wskazuje 'wartosc' i dodaje na koniec znak nowej linii.
*/
void wyswietl_zmienna(int *wartosc, int dlugosc)
{
    int index = dlugosc - 1;

    while (index > 0 && wartosc[index] == 0)
    {
        index--;
    }

    while (index >= 0)
    {
        printf("%d", wartosc[index]);
        index--;
    }

    printf("\n");
}

/**
    wartosc - adres wskaznika do tablicy zawierajacej dziesietny zapis zmiennej,
    od najmniej do najbardziej znaczacych cyfr

    dlugosc - adres pamieci przechowujacej ilosc cyfr tworzacych wartosc zmiennej

    Zeruje wartosc zmiennej zawartej w tablicy 'wartosc' oraz zmienia jej dlugosc
    przechowywana w pamieci o adresie 'dlugosc' na jeden.
*/
void wyzeruj_zmienna(int **wartosc, int *dlugosc)
{
    int *zero;
    zero = (int *) calloc(1000, sizeof(int));

    free(*wartosc);
    *wartosc = zero;
    *dlugosc = 1;
}

/**
    wartosc - wskaznik do tablicy zawierajacej dziesietny zapis zmiennej,
    od najmniej do najbardziej znaczacych cyfr

    dlugosc - adres pamieci przechowujacej ilosc cyfr tworzacych wartosc zmiennej

    Odejmuje jeden od wartosci zmiennej reprezentowanej przez tablice 'wartosc' oraz
    aktualizuje dlugosc wartosci zmiennej.
*/
void zmniejsz_o_jeden(int *wartosc, int *dlugosc)
{
    int index = 0;

    while (wartosc[index] == 0)
    {
        index++;
    }

    wartosc[index] -= 1;
    index--;

    while (index >= 0)
    {
        wartosc[index] = 9;
        index--;
    }

    if (wartosc[*dlugosc - 1] == 0 && *dlugosc - 1 != 0)
    {
        *dlugosc -= 1;
    }
}

/**
    wartosc - wskaznik do tablicy zawierajacej dziesietny zapis zmiennej,
    od najmniej do najbardziej znaczacych cyfr

    dlugosc - adres pamieci zawierajacej ilosc cyfr tworzacych wartosc zmiennej

    Dodaje jeden do wartosci zmiennej reprezentowanej przez tablice 'wartosc' oraz
    aktualizuje dlugosc wartosci zmiennej.
*/
void zwieksz_o_jeden(int *wartosc, int *dlugosc)
{
    wartosc[0] += 1;

    int index = 0;
    while (wartosc[index] >= 10)
    {
        wartosc[index] %= 10;
        wartosc[index + 1] += 1;
        index++;
    }

    if (wartosc[*dlugosc] != 0)
    {
        *dlugosc += 1;
    }
}

/**
    wartoscA - wskaznik do tablicy zawierajacej dziesietny zapis zmiennej 'A',
    od najmniej do najbardziej znaczacych cyfr

    dlugoscA - adres pamieci zawierajacej ilosc cyfr tworzacych wartosc zmiennej 'A'

    wartoscB - wskaznik do tablicy zawierajacej dziesietny zapis zmiennej 'B',
    od najmniej do najbardziej znaczacych cyfr

    dlugoscB - adres pamieci zawierajacej ilosc cyfr tworzacych wartosc zmiennej 'B'

    Dodaje do wartosci zmiennej 'B' reprezentowanej przez tablice 'wartoscB',
    wartosc zmiennej 'A' reprezentowanej przez tablice 'wartoscA'.
*/
void dodaj_a_do_b(int *wartoscA, int *dlugoscA, int **wartoscB, int *dlugoscB)
{
    // wskaznik do tablicy z wynikiem dodawania
    int *wynik;
    wynik = (int *) calloc(1000, sizeof(int));

    // zmienna iteracyjna
    int index = 0;
    // dlugosc dluzszej z dodawanych zmiennych
    int wieksza_dlugosc = max(*dlugoscA, *dlugoscB);

    // dodanie podanych liczb i wpisanie wyniku do tablicy na ktora wskazuje 'wynik'
    while (index < wieksza_dlugosc)
    {
        wynik[index] += wartoscA[index] + (*wartoscB)[index];

        if (wynik[index] >= 10)
        {
            wynik[index] %= 10;
            wynik[index + 1] += 1;
        }

        index++;
    }

    // ewentualne zwiekszenie dlugosci wartosci zmiennej 'B'
    if (wynik[wieksza_dlugosc] != 0)
    {
        *dlugoscB = wieksza_dlugosc + 1;
    }
    else
    {
        *dlugoscB = wieksza_dlugosc;
    }

    /*
        zwolnienie pamieci przechowujacej poprzednia wartosc zmiennej 'B'
        i przypisanie jej wyniku dodawania podanych wartosci
    */
    free(*wartoscB);
    *wartoscB = wynik;
}

/**
    program - wskaznik do tablicy zawierajacej program

    index - wskaznik do pamieci zawierajacej indeks zmiennej sterujacej petla w tablicy 'program'

    wartosci - tablica o rozmiarze 26 zawierajaca adresy tablic
    przechowujacych wartosci zmiennych

    dlugosci - tablica o rozmiarze 26 zawierajaca dlugosci zmiennych

    Realizuje instrukcje 'Powtarzaj' w jezyku Petlik.
*/
void powtarzaj(char *program, int *index, int *wartosci[26], int dlugosci[26])
{
    // index zmiennej sterujacej w tablicy 'wartosci' i 'dlugosci'
    int zmienna_sterujaca = ((int) program[*index]) - 97;
    // index zmiennej sterujacej w tablicy przechowujajcej caly program
    int index_sterujacej = *index;
    *index += 1;

    if (program[*index] == ')') // sprawdzenie czy petla jest pusta tzn. jest postaci '(zmienna)'
    {
        wyzeruj_zmienna(&wartosci[zmienna_sterujaca], &dlugosci[zmienna_sterujaca]);
        // wyjscie z petli
        *index += 1;
    }
    else // realizacja niepustej petli
    {
        /*
            pierwszy index w tablicy 'program' za petla odpowiadajaca zmiennej sterujacej

            UWAGA: zmienna musi zostać zainicjalizowana wartością aby uniknąć ostrzezenia podczas
            kompilacji. Na pozniejszym etapie jej wartosc jest zawsze zmieniana na poprawna!
        */
        int koniec_petli = -1;

        // realizacja petli dopoki zmienna sterujaca jest wieksza od zera
        while (!jest_zerem(wartosci[zmienna_sterujaca], dlugosci[zmienna_sterujaca]))
        {   
            // zmniejszenie o jeden zmiennej sterujacej
            zmniejsz_o_jeden(wartosci[zmienna_sterujaca], &dlugosci[zmienna_sterujaca]);

            // wykonywanie instrukcji do napotkania nawiasu konczacego dana petle
            while (program[*index] != ')')
            {
                // wyszukiwanie konca danej lub poczatku nowej petli
                while (program[*index] != '(' && program[*index] != ')')
                {
                    int zmienna = ((int) program[*index]) - 97;
                    zwieksz_o_jeden(wartosci[zmienna], &dlugosci[zmienna]);
                    *index += 1;
                }

                if (program[*index] == ')') // optymalizacja danej petli
                {
                    for (int x = index_sterujacej + 1; x < *index; x++)
                    {
                        // index zmiennej w tablicy 'wartosci' i 'dlugosci'
                        int zmienna = ((int) program[x]) - 97;
                        // dodanie wartosci zmiennej sterujacej do zmiennej umieszczonej w petli
                        dodaj_a_do_b(wartosci[zmienna_sterujaca], &dlugosci[zmienna_sterujaca],
                                     &wartosci[zmienna], &dlugosci[zmienna]);
                    }

                    // wyzerowanie zmiennej sterujacej
                    wyzeruj_zmienna(&wartosci[zmienna_sterujaca], &dlugosci[zmienna_sterujaca]);
                }
                else // wejscie w nowa petle
                {
                    *index += 1;
                    powtarzaj(program, index, wartosci, dlugosci);
                }
            }

            // zapisanie pierwszego indexu w tablicy 'program' za dana petla
            koniec_petli = *index + 1;

            // powrot na poczatek danej petli
            *index = index_sterujacej + 1;
        }

        // wyjscie z petli odpowiadajacej zmiennej sterujacej
        *index = koniec_petli;
    }
}

int main(void)
{
    // tablica przechowujaca wskazniki do wartosci zmiennych
    int *wartosci[26];
    // tablica przechowujaca dlugosci wartosci zmiennych
    int dlugosci[26];

    // pierwszy znak kolejnych linii wejscia
    int pierwszy_znak;

    inicjalizuj_zmienne(wartosci, dlugosci);

    // zczytywanie kolejnych linijek wejscia
    pierwszy_znak = getchar();
    while (pierwszy_znak != EOF)
    {
        if (pierwszy_znak == 61) // wyswietlenie wartosci zmiennej
        {
            int zmienna = getchar() - 97;
            wyswietl_zmienna(wartosci[zmienna], dlugosci[zmienna]);
            getchar();
        }
        else // realizacja programu zapisanego w linijce
        {
            // zczytanie tresci programu
            ungetc(pierwszy_znak, stdin);
            char *program;
            program = (char *) malloc(INT_MAX * sizeof(char));
            fgets(program, INT_MAX, stdin);

            // zmienna iteracyjna
            int index = 0;

            // realizacja instrukcji programu
            while (program[index] != '\n')
            {
                if (program[index] == '(') // wejscie w petle
                {
                    index += 1;
                    powtarzaj(program, &index, wartosci, dlugosci);
                }
                else // zwiekszenie wartosci zmiennej o jeden
                {
                    // indeks odpowiadajacy zmiennej w tablicach wartosci i dlugosci
                    int zmienna = ((int) program[index]) - 97;
                    // zwiekszenie wartosci zmiennej o jeden
                    zwieksz_o_jeden(wartosci[zmienna], &dlugosci[zmienna]);
                    index++;
                }
            }

            // zwolnienie pamieci zawierajacej wykonany program
            free(program);
        }

        // zczytanie pierwszego znaku kolejnej linijki wejscia
        pierwszy_znak = getchar();
    }

    zwolnij_pamiec_zmiennych(wartosci);

    return 0;
}