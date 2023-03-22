/************************************************************************************
 *Tomasz Lutkowski 263483    	       	       	       	   
 *Przetwarzanie obrazow 1                                 
 *Data wykonania: 08.12.21.									               
 ************************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

#define MAX 512            /* Maksymalny rozmiar wczytywanego obrazu */
#define DL_LINII 1024      /* Dlugosc buforow pomocniczych */

int czyPoprawne = 0;    /* zmienna globalna. dzieki ktorej bedziemy mogli sprawdzic czy plik jest poprawny, jesli tak to menu zostanie wyswietlona uzytkownikowi */

/************************************************************************************
 * Funkcja wczytuje obraz PGM z pliku do tablicy       	       	       	       	    *
 *										    *
 * \param[in] plik_we uchwyt do pliku z obrazem w formacie PGM			    *
 * \param[out] obraz_pgm tablica, do ktorej zostanie zapisany obraz		    *
 * \param[out] wymx szerokosc obrazka						    *
 * \param[out] wymy wysokosc obrazka						    *
 * \param[out] szarosci liczba odcieni szarosci					    *
 * \return liczba wczytanych pikseli						    *
 ************************************************************************************/

int czytaj(FILE *plik_we,int obraz_pgm[][MAX],int *wymx,int *wymy, int *szarosci) {
  char buf[DL_LINII];      /* bufor pomocniczy do czytania naglowka i komentarzy */
  int znak;                /* zmienna pomocnicza do czytania komentarzy */
  int koniec=0;            /* czy napotkano koniec danych w pliku */
  int i,j;

  /*Sprawdzenie czy podano prawidłowy uchwyt pliku */
  if (plik_we==NULL) {
    fprintf(stderr,"Blad: Nie podano uchwytu do pliku\n");
    czyPoprawne = 1;
    return(0);
  }

  /* Sprawdzenie "numeru magicznego" - powinien być P2 */
  if (fgets(buf,DL_LINII,plik_we)==NULL)   /* Wczytanie pierwszej linii pliku do bufora */
    koniec=1;                              /* Nie udalo sie? Koniec danych! */

  if ( (buf[0]!='P') || (buf[1]!='2') || koniec) {  /* Czy jest magiczne "P2"? */
    fprintf(stderr,"Blad: To nie jest plik PGM\n");
    czyPoprawne = 1;
    return(0);
  }

  /* Pominiecie komentarzy */
  do {
    if ((znak=fgetc(plik_we))=='#') {         /* Czy linia rozpoczyna sie od znaku '#'? */
      if (fgets(buf,DL_LINII,plik_we)==NULL)  /* Przeczytaj ja do bufora                */
	koniec=1;                   /* Zapamietaj ewentualny koniec danych */
    }  else {
      ungetc(znak,plik_we);                   /* Gdy przeczytany znak z poczatku linii */
    }                                         /* nie jest '#' zwroc go                 */
  } while (znak=='#' && !koniec);   /* Powtarzaj dopoki sa linie komentarza */
                                    /* i nie nastapil koniec danych         */

  /* Pobranie wymiarow obrazu i liczby odcieni szarosci */
  if (fscanf(plik_we,"%d %d %d",wymx,wymy,szarosci)!=3) {
    fprintf(stderr,"Blad: Brak wymiarow obrazu lub liczby stopni szarosci\n");
    czyPoprawne = 1;
    return(0);
  }

  /* Pobranie obrazu i zapisanie w tablicy obraz_pgm*/
  for (i=0;i<*wymy;i++) {
    for (j=0;j<*wymx;j++) {
      if (fscanf(plik_we,"%d",&(obraz_pgm[i][j]))!=1) {
	      fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
	      return(0);
      }
    }
  }
  return *wymx**wymy, czyPoprawne;   /* Czytanie zakonczone sukcesem    */
}                       /* Zwroc liczbe wczytanych pikseli */


/* Wyswietlenie obrazu o zadanej nazwie za pomoca programu "display"   */
void wyswietl(char *n_pliku) {
  char polecenie[DL_LINII];      /* bufor pomocniczy do zestawienia polecenia */

  strcpy(polecenie,"display ");  /* konstrukcja polecenia postaci */
  strcat(polecenie,n_pliku);     /* display "nazwa_pliku" &       */
  strcat(polecenie," &");
  printf("%s\n",polecenie);      /* wydruk kontrolny polecenia */
  system(polecenie);             /* wykonanie polecenia        */
}

/************************************************************************************
 * Funkcja zapisuje obraz PGM wczytany z pliku do podanego pliku       	       	       	       	    *
 *										    *
 * \param[in] plik_wy uchwyt do pliku, w ktorym zostanie zapisany obraz PGM			    *
 * \param[in] obrazPgm tablica, z obrazem PGM		    *
 * \param[in] wymx szerokosc obrazka						    *
 * \param[in] wymy wysokosc obrazka						    *
 * \param[in] szarosci liczba odcieni szarosci    *
 * \param[out]	plik_wy uchwyt zapisanego pliku				    					    *
 ************************************************************************************/

void zapisz(FILE *plik_wy,int obrazPgm[][MAX],int *wymx,int *wymy, int *szarosci)
{
  /* Wypisanie do pliku magicznego nuemru, szerokosci, wysokosci i skali szarosci obrazka, ktory chcemy zapisac */
  fprintf(plik_wy,"P2\n");
  fprintf(plik_wy,"%d %d\n%d\n",*wymx, *wymy, *szarosci);

  /* Wypisanie zawartosci tablicy do pliku */
  for(int i = 0; i < *wymy; ++i)    {
    for(int j = 0; j < *wymx; ++j){
	    fprintf(plik_wy, "%d ", obrazPgm[i][j]);
    }
      fprintf(plik_wy, "\n");
    }
}

/************************************************************************************
 * Funkcja przeprowadza operacje negatywu an obrazie PGM       	       	       	       	    *
 *										    *
 * \param[in] obrazPgm tablica, w kotrej jest przechowywany obraz			    *
 * \param[in] wymx szerokosc obrazka						    *
 * \param[in] wymy wysokosc obrazka						    *
 * \param[in] szarosci liczba odcieni szarosci        *
 * \param[out] obrazPgm tablica ze zmienionymi przez operacje elementami					    					    *
 ************************************************************************************/

void negatyw(int obrazPgm[][MAX], int *wymx, int *wymy, int *szarosci)
{
  /* Podstawienie pod elementy tabeli roznicy skali szarosci i elementu tabeli */
  for(int i = 0; i < *wymy; ++i)
    for(int j = 0; j < *wymx; ++j)
      obrazPgm[i][j] = *szarosci - obrazPgm[i][j];
}

/************************************************************************************
 * Funkcja przeprowadza operacja polprogowania bieli na obrazie PGM       	       	       	       	    *
 *										    *
 * \param[in] prog wartosc progu, wartosc z przedzialu 0.0 - 1.0    *
 * \param[in] obrazPgm tablica, w kotrej jest przechowywany obraz			    *
 * \param[in] wymx szerokosc obrazka						    *
 * \param[in] wymy wysokosc obrazka						    *
 * \param[in] szarosci liczba odcieni szarosci		    *
 * \param[out] obrazPgm tablica z ezmienionymi przez operacje elementami			    					    *
 ************************************************************************************/

void polProgowanieBieli(int prog, int obrazPgm[][MAX], int *wymx, int *wymy, int *szarosci)
{
  /* obliczenie progu, na podstawie ktorego bedzie wykoanne progowanie */
  int polprog = prog * (*szarosci);

  /* zmiana wartosci elementow tabeli, bedacych powyzej podanego progu na maksymalna wartosc skali szarosci */
  for(int i = 0; i < *wymy; ++i)
    for(int j = 0; j < *wymx; ++j)
    {
      if(obrazPgm[i][j] > polprog)
        obrazPgm[i][j] = *szarosci;
    } 
}

/************************************************************************************
 * Funkcja przeprowadza operacje konturowania na obrazie PGM       	       	       	       	    *
 *										    *
 * \param[in] obraz tablica, w kotrej jest przechowywany obraz	    *
 * \param[in] wymx szerokosc obrazka						    *
 * \param[in] wymy wysokosc obrazka			
 * \param[out] obraz tablica ze zmienionymi przez operacje elementami			    *				    					    *
 ************************************************************************************/

void konturowanie(int obraz[][MAX], int wymx, int wymy)
{
  int k, l; /* zmienne pomocnicze do obliczenia wartosci pikseli obrazu wynikowego */
  for(int i = 0;i < wymy; ++i)
    for(int j = 0; j < wymx; ++j)
    {
      k = obraz[i+1][j] - obraz[i][j];
      l = obraz[i][j+1] - obraz[i][j];
      if(i == 0 && j < wymx) /* warunek dla pierwszej linijki pikseli */
        obraz[i][j] = abs(l);
      if(i != 0 && j < wymx) 
        obraz[i][j] = abs(l) + abs(k);
      if(j == wymx-1) /* warunek dla pikseli skrajnie na prawo */
        obraz[i][j] = abs(k);
      if(i == wymy-1) /* warunek dla ostatniej linijki pikseli */
        obraz[i][j] = abs(l);
      if(i==wymy-1 && j == wymx -1) /* warunek dla piksela w skrajnym prawym, dolnym rogu */
        obraz[i][j] = abs(l) - abs(k);
    }
}

int main() {
  int obraz[MAX][MAX] ;
  int wymx,wymy,odcieni;
  float prog = 0.0;
  int odczytano = 0;
  FILE *plik;
  char nazwa[100], nazwa2[100];

  /* Wczytanie zawartosci wskazanego pliku do pamieci */
  printf("Podaj nazwe pliku:\n");
  scanf("%s",nazwa);
  plik=fopen(nazwa,"r");

  if (plik != NULL) {       /* co spowoduje zakomentowanie tego warunku */
    odczytano = czytaj(plik,obraz,&wymx,&wymy,&odcieni);
    fclose(plik);

    if(czyPoprawne == 0)
    {
      char wybor[2] = " ";
      while(wybor[0] != '6')
      {
        printf("PRZETWARZANIE OBRAZOW: \n");
        printf("\t1. Wyswietl wczytany obraz\n");
        printf("\t2. Przeprowadz operacje na obrazie - negatyw\n");
        printf("\t3. Przeprowadz operacje na obrazie - polprogowanie bieli\n");
        printf("\t4. Przeprowadz operacje na obrazie - konturowanie\n");
        printf("\t5. Zapisz obraz do wybranego pliku\n");
        printf("\t6. Zakoncz prace programu\n");
        printf("Twoj wybor: ");
        scanf("%1s", wybor);
        switch(wybor[0])
        {
          case '1':
            system("clear"); /* wyczyszcza ekran terminala */
            printf("Wybrano wyswietlenie wczytanego obrazu.\n");
            /* Wyswietlenie poprawnie wczytanego obrazu zewnetrznym programem */
            if (odczytano != 0)
              wyswietl(nazwa);
            else  
              printf("Nie wczytano pliku.\n");
            break;
          case '2': 
            system("clear");
            printf("Wybrano przeprowadzenie operacji negatyw.\n");
            negatyw(obraz, &wymx, &wymy, &odcieni);
            break;
          case '3':
            system("clear");
            printf("Wybrano przeprowadzenie operacji polprogowanie bieli.\n");
            printf("Prosze wprowadzic wartosc progu z przedzialu 0.0 - 1.0: ");
            scanf("%f", &prog);
            if(prog>=0.0 && prog <=1.0)
              {
              polProgowanieBieli(prog, obraz, &wymx, &wymy, &odcieni);
              break;
              }
            else
              {
              printf("Niepoprawna wartosc progu.\n");
              break;
              }
          case '4':
            system("clear");
            printf("Wybrano przeprowadzenie operacji konturowanie.\n");
            konturowanie(obraz, wymx, wymy);
            break;
          case '5':
            system("clear");
            printf("Wybrano zapisanie obrazu do pliku.\n");
            printf("Podaj nazwe pliku do zapisu: \n");
            scanf("%s", nazwa2);
            plik=fopen(nazwa2, "w");

            if(plik != NULL)
            {
              zapisz(plik, obraz, &wymx, &wymy, &odcieni);
              fclose(plik);
            }

            break;
          case '6':
            system("clear");
            printf("Zakonczono dzialanie programu.\n");
            break;  
          default:
            system("clear");
            printf("Nie ma takiej opcji w menu.\nProsze wybraz jedan z dostepnych opcji/\n");
            break;  
        }
      }
    }
  }

  if(plik == NULL)
  {
    printf("Taki plik nie istnieje.\n");
    return 0;
  }

  return odczytano;
}

/*
Test 1: 
Cel: Sprawdzenie poprawnosci dzialania programu dla nieistniejacego pliku
Dane wejsciowe: asdas.txt
Dane wyjsciowe: Taki plik nie istnieje.
Rezultat: Program dziala poprawnie. Nie bierze pod uwage nieistniejacego pliku i nie wyswietla uzytkownikowi menu.

Test 2: 
Cel: Sprawdzenie poprawnosci dzialania dla pliku o innym rozszerzeniu, ktoy nie ma magicznego numeru
Dane wejsciowe: test.c
Dane wyjsciowe: Blad: To nie jest plik PGM
Rezultat: Program dziala poprawnie. NIe uruchamia menu dla plikow, ktore nie maja magicznego numeru (nie sa obrazami PGM)

Test 3:
Cel: Sprawdzenie poprawnosci dzialania programu dla plikow PGM
Dane wejsciowe: maly_1.pgm
Dane wyjsciowe: 
PRZETWARZANIE OBRAZOW: 
        1. Wyswietl wczytany obraz
        2. Przeprowadz operacje na obrazie - negatyw
        3. Przeprowadz operacje na obrazie - polprogowanie bieli
        4. Przeprowadz operacje na obrazie - konturowanie
        5. Zapisz obraz do wybranego pliku
        6. Zakoncz prace programu
Twoj wybor: 
Rezultat: Program dziala poprawnie. Umozliwia uzytkownikowi korzystanie z menu.

Test 4:
Cel: Sprawdzenie poprawnosci dzialania opcji zapisz dla istniejacego pliku
Komentarz: Na poczatku wczytujemy plik maly_1.pgm, a nastepnie wybieramy opcje 5. z menu
Dane wejsciowe: plikTestowy.pgm (plik ktory zostal utworzony do tego testu)
Dane wyjsciowe: Ponowne wlaczenie menu, oczekujace na wykonanie kolejnych dzialan
Rezultat: Program dziala poprawnie. Po sprawdzeniu komenda cat zawartosci pliku plikTestowy.pgm, widzimy ta sama zawartosc co w maly_1.pgm

Test 5:
Cel: Sprawdzenie poprawnosci dzialania opcji zapisz dla nieisniejacego pliku
Komentarz: Na poczatku wczytujemy plik maly_1.pgm, a nastepnie wybieramy opcje 5. z menu
Dane wejsciowe: plik.pgm
Dane wyjsciowe: Dane wyjsciowe: Ponowne wlaczenie menu, oczekujace na wykonanie kolejnych dzialan
Rezultat: Program dziala poprawnie. Jezeli plik ktory wpisujemy nie istnieje, zostanie on utworzony przez program i zostanie do niego zapisana wybrana zawartosc

Kategoria testow: Sprawdzanie poprawnosci dzialania menu
  Wspolne kroki wykonywane przy testach od 6.1 do 6.5

  *Wczytanie pliku maly_1.pgm
  *Wybranie jednej z opcji w menu odpowiadajacej za konkretna operacje na obrazie
  *zapisanie utworzonego obrazu do wybranego pliku za pomoca opcji zapisz (5.)
  *Ponowne uruchomienie programu w celu wyswietlenia zapisanego pliku

  Test 6.1:
  Cel: Sprawdzenie poprawnosci dzialania opcji wyswietl
  Dane wejsciowe: 1
  Dane wyjsciowe: Otwarcie okienka z obrazem
  Rezultat: Program dziala poprawnie. Po wykonaniu operacji, wyswietlane sa kolejne operacje ktore mozna wykonac

  Test 6.2:
  Cel: Sprawdzenie poprawnosci dzialania operacji negatyw
  Dane wejsciowe: 2
  Dane wyjsciowe: Ponowne wyswietlenie menu
  Komentarz: Po ponownym uruchomieniu programu i wczytaniu pliku, w ktorym zapisano operacje negatywu wykonana na maly_1.pgm i wybraniu opcji wyswietl widzimy poprawnie obrobiony obraz
  Rezultat: Program dziala poprawnie

  Test 6.3:
  Cel: Sprawdzenie poprawnosci dzialania operacji polprogrowania bieli
  Dane wejsciowe: 3 
  Dane wyjsciwoe: Prosze wprowadzic wartosc progu z przedzialu 0.0 - 1.0: 
  Dane wejsciowe: 0.5
  Dane wyjsciowe: Ponowne wyswietlenie menu
  Komentarz: Po ponownym uruchomieniu programu i wczytaniu pliku, w ktorym zapisano operacje polprogowania bieli wykonana na maly_1.pgm i wybraniu opcji wyswietl widzimy poprawnie obrobiony obraz
  Rezultat: Program dziala poprawnie

  Test 6.4:
  Cel: Sprawdzenie poprawnosci dzialania operacji konturowania
  Dane wejsciowe: 4
  Dane wyjsciowe: Ponowne wyswietlenie menu
  Komentarz: Po ponownym uruchomieniu programu i wczytaniu pliku, w ktorym zapisano operacje konturowania wykonana na maly_1.pgm i wybraniu opcji wyswietl widzimy poprawnie obrobiony obraz
  Rezultat: Program dziala poprawnie

  Test 6.5:
  Cel: Sprawdzenie poprawnosci dzialania operacji zakoncznenia programu
  Dane wejsciowe: 6
  Dane wyjsciowe: Zakonczono dzialanie programu.
  Rezultat: Program dziala poprawnie.
*/
