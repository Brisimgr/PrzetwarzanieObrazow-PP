/********************************************************************/
/*                                                                  */
/* ALTERNATYWNA DO PRZEDSTAWIONEJ NA WYKLADZIE WERSJA OPRACOWANIA   */
/* PARAMETROW WYWOLANIA PROGRAMU UWZGLEDNIAJACA wybor Z PARAMETRAMI */
/* Z ODPOWIEDNIO ZAPROPONOWANYMI STRUKTURAMI DANYCH PRZEKAZUJACYMI  */
/* WCZYTANE USTAWIENIA                                              */
/*                                    COPYRIGHT (c) 2007-2020 KCiR  */
/*                                                                  */
/* Autorzy udzielaja kazdemu prawa do kopiowania tego programu      */
/* w calosci lub czesci i wykorzystania go w dowolnym celu, pod     */
/* warunkiem zacytowania zrodla                                     */
/*                                                                  */
/********************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"../inc/obslugaPlikow.h"
#include"../inc/filtry.h"
#include"../inc/opcje.h"
#define W_OK 0                   /* wartosc oznaczajaca brak bledow */
#define B_NIEPOPRAWNAOPCJA -1    /* kody bledow rozpoznawania opcji */
#define B_BRAKNAZWY   -2
#define B_BRAKWARTOSCI  -3
#define B_BRAKPLIKU   -4
#define B_BRAKFILTRU -5
#define FILTR 3 /* rozmiar tablicy z filtrami */

extern int czyPoprawne;    /* zmienna globalna. dzieki ktorej bedziemy mogli sprawdzic czy plik jest poprawny, jesli tak to menu zostanie wyswietlona uzytkownikowi */

/*******************************************************/
/* Funkcja inicjuje strukture obrazazywana przez wybor */
/* PRE:                                                */
/*      poprawnie zainicjowany argument wybor (!=NULL) */
/* POST:                                               */
/*      "wyzerowana" struktura wybor wybranych opcji   */
/*******************************************************/

void wyzeruj_opcje(t_opcje * wybor) {
  wybor->plik_we=NULL;
  wybor->plik_wy=NULL;
  wybor->negatyw=0;
  wybor->konturowanie=0;
  wybor->progowanie=0;
  wybor->wyswietlenie=0;
}

/************************************************************************/
/* Funkcja rozpoznaje wybor wywolania programu zapisane w tablicy argv  */
/* i zapisuje je w strukturze wybor                                     */
/* Skladnia opcji wywolania programu                                    */
/*         program {[-i nazwa] [-o nazwa] [-p liczba] [-n] [-r] [-d] }  */
/* Argumenty funkcji:                                                   */
/*         argc  -  liczba argumentow wywolania wraz z nazwa programu   */
/*         argv  -  tablica argumentow wywolania                        */
/*         wybor -  struktura z informacjami o wywolanych opcjach       */
/* PRE:                                                                 */
/*      brak                                                            */
/* POST:                                                                */
/*      funkcja otwiera odpowiednie pliki, zwraca uchwyty do nich       */
/*      w strukturze wyb�r, do tego ustawia na 1 pola dla opcji, ktore  */
/*	poprawnie wystapily w linii wywolania programu,                 */
/*	pola opcji nie wystepujacych w wywolaniu ustawione sa na 0;     */
/*	zwraca wartosc W_OK (0), gdy wywolanie bylo poprawne            */
/*	lub kod bledu zdefiniowany stalymi B_* (<0)                     */
/* UWAGA:                                                               */
/*      funkcja nie sprawdza istnienia i praw dostepu do plikow         */
/*      w takich przypadkach zwracane uchwyty maja wartosc NULL         */
/************************************************************************/

int przetwarzaj_opcje(int argc, char **argv, t_opcje *wybor) {
  int i, prog, odczytano;
  char *nazwa_pliku_we, *nazwa_pliku_wy = NULL, *filtr;

  /* inicjacja roznych filtrow wykozystywanych w operacji splotu */
  int filtrUsre[FILTR][FILTR]={{1,1,1},{1,1,1},{1,1,1}}, filtrUsreWzmo[FILTR][FILTR]={{1,1,1},{1,2,1},{1,1,1}}, gradientRob[FILTR][FILTR]={{0,0,0},{-1,0,0},{0,1,0}}, maskaPrew[FILTR][FILTR]={{-1,-1,-1},{0,0,0},{1,1,1}};
  int maskaSob[FILTR][FILTR]={{-1,-2,-1},{0,0,0},{1,2,1}}, maskaNaro[FILTR][FILTR]={{1,1,1},{-1,-2,1},{-1,-1,1}}, laplasjany[FILTR][FILTR]={{-1,-1,-1},{-1,8,-1},{-1,-1,-1}};


  t_obraz *obraz;
  obraz = (t_obraz *) malloc(sizeof(t_obraz));
  wyzeruj_opcje(wybor);
  wybor->plik_wy=stdout;        /* na wypadek gdy nie podano opcji "-o" */

  for (i=1; i<argc; i++) {
    if (argv[i][0] != '-')  /* blad: to nie jest opcja - brak znaku "-" */
      return B_NIEPOPRAWNAOPCJA; 
    switch (argv[i][1]) {
    case 'i': {                 /* opcja z nazwa pliku wejsciowego */
      if (++i<argc) {   /* wczytujemy kolejny argument jako nazwe pliku */
	      nazwa_pliku_we=argv[i];
	      if (strcmp(nazwa_pliku_we,"-")==0) /* gdy nazwa jest "-"        */
	        wybor->plik_we=stdin;            /* ustwiamy wejscie na stdin */
	      else                             /* otwieramy obrazazany plik   */
	        wybor->plik_we=fopen(nazwa_pliku_we,"r");
      }
      else 
	      return B_BRAKNAZWY;                   /* blad: brak nazwy pliku */
      break;
    }
    case 'o': {                 /* opcja z nazwa pliku wyjsciowego */
      if (++i<argc) {   /* wczytujemy kolejny argument jako nazwe pliku */
	      nazwa_pliku_wy=argv[i];
	      if (strcmp(nazwa_pliku_wy,"-")==0)/* gdy nazwa jest "-"         */
	        wybor->plik_wy=stdout;          /* ustwiamy wyjscie na stdout */
	      else                          /* otwieramy obrazazany plik    */
	        wybor->plik_wy=fopen(nazwa_pliku_wy,"w");
      }
      else 
	      return B_BRAKNAZWY;                   /* blad: brak nazwy pliku */
      break;
    }
    case 'p': {
      if (++i<argc) { /* wczytujemy kolejny argument jako wartosc progu */
	      if (sscanf(argv[i],"%d",&prog)==1) {
	        wybor->progowanie=1;
	        wybor->w_progu=prog;
	      }
        else
	        return B_BRAKWARTOSCI;     /* blad: niepoprawna wartosc progu */
      }
      else 
	      return B_BRAKWARTOSCI;             /* blad: brak wartosci progu */
      break;
    }
    case 'n': {                 /* mamy wykonac negatyw */
      wybor->negatyw=1;
      break;
    }
    case 'k': {                 /* mamy wykonac konturowanie */
      wybor->konturowanie=1;
      break;
    }
    case 'd': {                 /* mamy wyswietlic obraz */
      wybor->wyswietlenie=1;
      break;
    }
    case 's':           /* mamy wykonac splot */
    {
      if(++i<argc)
        {
        filtr = argv[i];
        wybor->splot = 1;
        }
      else  
        return B_BRAKFILTRU;
      break;
    }
    default:                    /* nierozpoznana opcja */
      return B_NIEPOPRAWNAOPCJA;
      break;
    } /* koniec switch */
  } /* koniec for */

  if (wybor->plik_we != NULL) 
  {       
    odczytano = czytaj(wybor->plik_we, obraz);
    fclose(wybor->plik_we);
    if(czyPoprawne == 0)
    {
      if(wybor->negatyw == 1) /* jezeli wywolano negatyw */
      {
        if(obraz->jakiObraz==0) /* czy obraz jest w formacie PPM */
          konwersja(obraz);
        negatyw(obraz);
      }
      if(wybor->konturowanie == 1) /* jezeli wywolano konturowanie */
      {
        if(obraz->jakiObraz==0) /* czy obraz jest w  formacie PPM */
          konwersja(obraz);
        konturowanie(obraz);
      }
      if(wybor->progowanie == 1) /* jezeli wywolano progowanie */
      {
        if(obraz->jakiObraz==0) /* czy obraz jest w formacie PPM */
          konwersja(obraz);
        polProgowanieBieli(wybor->w_progu, obraz);
      }
      if(wybor->wyswietlenie == 1) /* jezeli wywolano wysiwetlenie */
      {
        if(odczytano != 0) /* czy poprawnie wczytano plik */
          wyswietl(nazwa_pliku_we);
      }
      if(wybor->splot == 1) /* jezeli wywolano splot */
      {
        /* Warunki dla poszczegolnych filtrow */
        /* Jest to jednoczesnie kolejnosc w jakiej beda wykonywane opcje, niezaleznie od wpisania przez uzytkownika */
        if(strcmp(filtr,"s1")==0)
          splot(obraz, filtrUsre);
        else if(strcmp(filtr,"s2")==0)
          splot(obraz, filtrUsreWzmo);
        else if(strcmp(filtr,"s3")==0)
          splot(obraz, gradientRob);
        else if(strcmp(filtr,"s4")==0)
          splot(obraz, maskaPrew);
        else if(strcmp(filtr,"s5")==0)
          splot(obraz, maskaSob);
        else if(strcmp(filtr,"s6")==0)
          splot(obraz, maskaNaro);
        else if(strcmp(filtr,"s7")==0)
          splot(obraz, laplasjany);
        else  
          {
          printf("Niepoprawny filtr.\n");
          return B_NIEPOPRAWNAOPCJA;
          }
      }
      if(nazwa_pliku_wy != NULL) /* zapisanie obrazu, jezeli podano nazwe pliku */
        zapisz(wybor->plik_wy,obraz);
    }
    
    return W_OK;
    /* zwalnianie pamieci zaalokowanej w funkcji czytaj */
    free(obraz->obrazPgm);
    obraz->obrazPgm = NULL;
    free(obraz);
    obraz = NULL;

  }
  else
    return B_BRAKPLIKU; /* blad:  nie otwarto pliku wejsciowego  */
}
      
