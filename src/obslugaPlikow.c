/************************************************************************************
 *Tomasz Lutkowski 263483    	       	       	       	                              *
 *Przetwarzanie obrazow 2                                                           * 
 *Data wykonania: 10.01.22.									                                        *
 ************************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include "../inc/obslugaPlikow.h"

extern int czyPoprawne;    /* zmienna globalna. dzieki ktorej bedziemy mogli sprawdzic czy plik jest poprawny, jesli tak to menu zostanie wyswietlona uzytkownikowi */

/************************************************************************************
 * Funkcja wczytuje obraz PGM lub PPM z pliku do tablicy       	       	       	       	    *
 * \param[in] plik_we uchwyt do pliku z obrazem w formacie PGM			                *
 * \param[in] obraz struktura, zawierajaca informacje o obrazie		    				      *
 * \return liczba wczytanych pikseli						                                    *
 ************************************************************************************/

int czytaj(FILE *plik_we, t_obraz *obraz) {
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

  if ( (buf[0]!='P') || (buf[1]!='2') && (buf[1]!='3') || koniec) {  /* Czy jest magiczne "P2" lub "P3"? */
    fprintf(stderr,"Blad: To nie jest plik PGM lub PPM\n");
    czyPoprawne = 1;
    return(0);
  }

  if(buf[1]=='2') /* Wyroznienie z jakim formatem obrazu mamy do czynienia */
    {
    obraz->jakiObraz=1; /* Format PGM */
    }
  else if(buf[1]=='3')  
    {  
    obraz->jakiObraz=0; /* Format PPM */
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
  if (fscanf(plik_we,"%d %d %d", &(obraz->wymX), &(obraz->wymY), &(obraz->odcien))!=3) {
    fprintf(stderr,"Blad: Brak wymiarow obrazu lub liczby stopni szarosci\n");
    czyPoprawne = 1;
    return(0);
  }
  
  if(obraz->jakiObraz==0) /* Zwiekszenie rozmiarow obrazu dla obrazow kolorowych */
    obraz->wymX=obraz->wymX*3;

  obraz->obrazPgm = malloc(obraz->wymX*obraz->wymY*sizeof(int)); /* Alokacja pamieci dla tablicy dynamicznej, zawierajacej informaje o obrazie */
  int (*obrazPgm)[obraz->wymX]; 
  obrazPgm = (int(*)[obraz->wymX]) obraz->obrazPgm;

  /* Pobranie obrazu i zapisanie w tablicy obraz_pgm*/
  for (i=0;i<obraz->wymY;i++) {
    for (j=0;j<obraz->wymX;j++) {
      if (fscanf(plik_we,"%d",&(obrazPgm[i][j]))!=1) {
	      fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
	      return(0);
      }
    }
  }
  return obraz->wymX*obraz->wymY;   /* Czytanie zakonczone sukcesem    */
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
 * Funkcja zapisuje obraz PGM lub PPM wczytany z pliku do podanego pliku       	       	    *
 * \param[in] plik_wy uchwyt do pliku, w ktorym zostanie zapisany obraz PGM			    *
 * \param[in] obraz struktura, zawierajaca informacje o obrazie		                  *
 * \param[out]	plik_wy uchwyt zapisanego pliku				    					                *
 ************************************************************************************/

void zapisz(FILE *plik_wy, t_obraz *obraz)
{
  
  int (*obrazPgm)[obraz->wymX];
  obrazPgm = (int(*)[obraz->wymX]) obraz->obrazPgm;

  if(obraz->jakiObraz==1) /* Wersja zapisu obrazu dla obrazow w formacie PGM, bez zadnych konwersji */
    {
      fprintf(plik_wy,"P2\n");
      fprintf(plik_wy,"%d %d\n%d\n",obraz->wymX, obraz->wymY, obraz->odcien);
      for(int i = 0; i < obraz->wymY; ++i)
        {
          for(int j = 0; j < (obraz->wymX); ++j)
          {
            fprintf(plik_wy, "%d ", obrazPgm[i][j]);
          }
          fprintf(plik_wy, "\n");
        }      
      }
  else if(obraz->jakiObraz==2) /* Wersja zapisu obrazu dla obrazow po konwersji z formatu PPM na PGM */
  {
    fprintf(plik_wy,"P2\n");
    fprintf(plik_wy,"%d %d\n%d\n",obraz->wymX/3, obraz->wymY, obraz->odcien);
    for(int i = 0; i < obraz->wymY; ++i)
      {
        for(int j = 0; j < (obraz->wymX/3); ++j)
        {
          fprintf(plik_wy, "%d ", obrazPgm[i][j]);
        }
        fprintf(plik_wy, "\n");
      }
  }
  else if(obraz->jakiObraz==0) /* Wersja zapisu obrazu dla obrazow w formacie PPM, bez zadnych konwersji */
  {
    fprintf(plik_wy,"P3\n");
    fprintf(plik_wy,"%d %d\n%d\n",obraz->wymX/3, obraz->wymY, obraz->odcien);
    for(int i = 0; i < obraz->wymY; ++i)
      {
        for(int j = 0; j < obraz->wymX; ++j)
        {
          fprintf(plik_wy, "%d ", obrazPgm[i][j]);
        }
        fprintf(plik_wy, "\n");
      }
  }
}