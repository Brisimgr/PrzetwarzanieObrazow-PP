/************************************************************************************
 *Tomasz Lutkowski 263483    	       	       	       	                              *
 *Przetwarzanie obrazow 2                                                           * 
 *Data wykonania: 10.01.22.									                                        *
 ************************************************************************************/

/************************************************************************************
*Funkcje sa wykonywane w podanej kolejnosci (odgornie narzuconej w programie):      *
*1. wczytanie obrazu                                                                *
*2. negatyw                                                                         *
*3. konturowanie                                                                    *
*4. polprogowanie bieli                                                             *
*5. wyswietlanie                                                                    *
*6. splot                                                                           *
*7. zapisanie obrazu									                                              *
************************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "../inc/obslugaPlikow.h"
#include "../inc/filtry.h"
#include "../inc/opcje.h"

int czyPoprawne = 0;

int main(int argc, char ** argv) {
  t_opcje opcje;
  int kod_bledu;
  
  kod_bledu=przetwarzaj_opcje(argc,argv,&opcje); /* ustala czy wystapil blad */

  if (kod_bledu)
    printf("Blad nr %d\n", kod_bledu);
  else
    printf("Opcje poprawne\n");

  return kod_bledu;
}

/*
Test 1: 
Cel: Sprawdzenie poprawnosci dzialania funkcji wczytaj oraz zapisz
Dane wejsciowe: ./przetwarzanieObrazow -i kubus.ppm -o zapis.ppm
Dane wyjsciowe: Opcje poprawne
Rezultat: Program dziala poprawnie. W pliku zapis.ppm jest ten sam obrazek co w kubus.ppm

Test 2:
Cel: Sprawdzenie poprawnosci dzialania programu dla nieistniejacego pliku
Dane wejsciowe: ./przetwarzanieObrazow -i asdad.ads -o zapis.ppm
Dane wyjsciowe: Blad nr -4
Rezultat: Program dziala poprawnie. Blad nr -4 oznacza brak pliku.

Test 3: 
Cel: Sprawdzenie poprawnosci dzialania dla pliku o innym rozszerzeniu, ktoy nie ma magicznego numeru
Dane wejsciowe: ./przetwarzanieObrazow -i opcje.c -o zapis.ppm
Dane wyjsciowe: Blad: To nie jest plik PGM. Opcje poprawne
Rezultat: Program dziala poprawnie. Wyswietlony zostaje komunikat o blednym formacie, mimo ze opcje zostaly poprawnie wprowadzone. Zawartosc pliku zapis.ppm nie zmenia sie.

Test 4:
Cel: Sprawdzenie poprawnosci dzialania opcji zapisz dla nieisniejacego pliku
Dane wejsciowe: ./przetwarzanieObrazow -i kubus.ppm -o zapis2.ppm
Dane wyjsciowe: Opcje poprawne
Rezultat: Program dziala poprawnie. Jezeli plik ktory wpisujemy nie istnieje, zostanie on utworzony przez program i zostanie do niego zapisana wybrana zawartosc

Test 5: 
Cel: Sprawdzenie poprawnosci dzialania funkcji negatyw 
Dane wejsciowe: ./przetwarzanieObrazow -i maly_2.pgm -n -o zapis.pgm
Dane wyjsciowe: Opcje poprawne
Rezultat: Program dziala poprawnie.

Test 5.1:
Cel: Sprawdzenie poprawnosci dzialania funkcji negatyw dla pliku o formacie PPM
Dane wejsciowe: ./przetwarzanieObrazow -i kubus.ppm -n -o zapis.ppm
Dane wyjsciowe: Opcje poprawne
Rezultat: Program dziala poprawnie. kubus.ppm zostaje najpierw przekonwertowany na obraz o odcieniach szarosci, dopiero po tej operacji przeprowadzony zostaje negatyw.

Test 6:
Cel : Sprawdzenie poprawnosci dzialania funkcji konturowanie
Dane wejsciowe: ./przetwarzanieObrazow -i maly_2.pgm -k -o zapis.pgm
Dane wyjsciowe: Opcje poprawne
Rezultat: Program dziala poprawnie 

Test 6.1:
Cel: Sprawdzenie poprawnosci dzialania funkcji konturowanie dla pliku o formacie PPM
Dane wejsciowe: ./przetwarzanieObrazow -i kubus.ppm -k -o zapis.ppm
Dane wyjsciowe: Opcje poprawne
Rezultat: Program dziala poprawnie. kubus.ppm zostaje najpierw przekonwertowany na obraz o odcieniach szarosci, dopiero po tej operacji przeprowadzone zostaje konturowanie.

Test 7:
Cel: Sprawdzenie poprawnosci dzialania funkcji polprogowanie bieli
Dane wejsciowe: ./przetwarzanieObrazow -i maly_2.pgm -p 50 -o zapis.pgm
Dane wyjsciowe: Opcje poprawne
Rezultat: Program dziala poprawnie

Test 7.1:
Cel: Sprawdzenie poprawnosci dzialania funkcji polprogowanie bieli dla pliku o formacie PPM
Dane wejsciowe: ./przetwarzanieObrazow -i kubus.ppm -p 50 -o zapis.ppm
Dane wyjsciowe: Opcje poprawne
Rezultat: Program dziala poprawnie. kubus.ppm zostaje najpierw przekonwertowany na obraz o odcieniach szarosci, dopiero po tej operacji przeprowadzone zostaje polprogowanie bieli.

Test  8: 
Cel: Sprawdzenie poprawnosci dzialania funkcji display
Dane wejsciowe: ./przetwarzanieObrazow -i kubus.ppm -d -o zapis.ppm
Dane wyjsciowe: display kubus.ppm & | Opcje poprawne
Rezultat: Program dziala poprawnie

Test 9:
Cel: Sprawdzenie poprawnosci dzialania funkcji splot
Dane wesjciowe: ./przetwarzanieObrazow -i kubus.ppm -s (s1/s2/s3/s4/s5/s6/s7) -o zapis.ppm
Dane wysjciowe: Opcje poprawne.
Rezultat: Program dziala poprawnie dla kazdego filtru.

Test 10: 
Cel: Spradzenie poprawnosci kolejnosci wykonywania operacji.
Komentarz:
Funkcje sa wykonywane w podanej kolejnosci (odgornie narzuconej w programie):
1. wczytanie obrazu
2. negatyw
3. konturowanie
4. polprogowanie bieli
5. wyswietlanie
6. splot
7. zapisanie obrazu
Dane wejsciowe: ./przetwarzanieObrazow -i kubus.ppm -n -k -p 50 -o zapis.ppm
Dane wyjsciowe: Opcje poprawne.
Rezultat: Program dziala poprawnie. Kolejnosc wpisania opcji nie ma znaczenia, operacje i tak wykonaja sie w kolejnosci podane powyzej.

Test 11:
Cel: Sprawdzenie komunikaty o braku wczytywanego pliku.
Dane wejsciowe: ./przetwarzanieObrazow -i 
Dane wyjsciowe: Blad nr -2
Rezultat: Program dziala poprawnie. Blad -2 oznacza brak nazwy wczytywanego pliku.

Test 12: 
Cel: Sprawdzenie poprawnosci funkcji zapisz na wyjscie stdout
Dane wejsciowe: ./przetwarzanieObrazow -i maly_2.pgm -o -
Dane wyjsciowe: 
P2
20 10
8
0 0 0 0 0 0 0 0 0 0 7 7 7 7 7 7 7 7 7 7 
0 0 0 0 0 0 0 0 0 0 7 7 7 7 7 7 7 7 7 7 
0 0 0 0 0 0 0 0 0 0 7 7 7 7 7 7 7 7 7 7 
0 0 0 0 0 0 0 0 0 0 7 7 7 7 7 7 7 7 7 7 
0 0 0 0 0 0 0 0 0 0 7 7 7 7 7 7 7 7 7 7 
7 7 7 7 7 7 7 7 7 7 0 0 0 0 0 0 0 0 0 0 
7 7 7 7 7 7 7 7 7 7 0 0 0 0 0 0 0 0 0 0 
7 7 7 7 7 7 7 7 7 7 0 0 0 0 0 0 0 0 0 0 
7 7 7 7 7 7 7 7 7 7 0 0 0 0 0 0 0 0 0 0 
7 7 7 7 7 7 7 7 7 7 0 0 0 0 0 0 0 0 0 0 
Opcje poprawne
Rezultat: Program dziala poprawnie. 

Test 13:
Cel: Sprawdzenie dzialania funkcji zapisz bez podanej nazwy pliku
Dane wejsciowe: ./przetwarzanieObrazow -i maly_2.pgm -o 
Dane wyjsciowe: Blad nr -2
Rezultat: Program dziala poprawnie. Blad -2 oznacza brak nazwy wczytywanego pliku.

Test 14: 
Cel: Sprawdzenie bledu o niepoprawnej opcji
Dane wejsciowe: ./przetwarzanieObrazow -i maly_2.pgm -u
Dane wyjsciowe: Blad nr -1
Rezultat: Program dziala poprawnie. Blad -1 oznacza niepoprawna opcje.

Test 15: 
Cel: Sprawdzenie funkcji polprogowanie bieli bez podania wartosci progu
Dane wejsciowe: ./przetwarzanieObrazow -i maly_2.pgm -p -o zapis.pgm
Dane wyjsciowe: Blad nr -3
Rezultat: Program dziala poprawnie. Blad numer 3 oznacza brak wartosci.

*/



