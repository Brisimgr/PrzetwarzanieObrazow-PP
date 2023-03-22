#include<stdio.h>
#include<stdlib.h>
#include "../inc/obslugaPlikow.h"
#include "../inc/filtry.h"

int czyPoprawne = 0;

int main() {
  t_obraz obraz;
  float prog = 0.0;
  int odczytano = 0;
  FILE *plik;
  char nazwa[100], nazwa2[100];

  /* Wczytanie zawartosci wskazanego pliku do pamieci */
  printf("Podaj nazwe pliku:\n");
  scanf("%s",nazwa);
  plik=fopen(nazwa,"r");

  if (plik != NULL) {       /* co spowoduje zakomentowanie tego warunku */
    odczytano = czytaj(plik, &obraz);
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
            if(obraz.jakiObraz==0)
              konwersja(&obraz);
            system("clear");
            printf("Wybrano przeprowadzenie operacji negatyw.\n");
            negatyw(&obraz);
            break;
          case '3':
            if(obraz.jakiObraz==0)
              konwersja(&obraz);
            system("clear");
            printf("Wybrano przeprowadzenie operacji polprogowanie bieli.\n");
            printf("Prosze wprowadzic wartosc progu z przedzialu 0.0 - 1.0: ");
            scanf("%f", &prog);
            if(prog>=0.0 && prog <=1.0)
              {
              polProgowanieBieli(prog, &obraz);
              break;
              }
            else
              {
              printf("Niepoprawna wartosc progu.\n");
              break;
              }
          case '4':
            if(obraz.jakiObraz==0)
              konwersja(&obraz);
            system("clear");
            printf("Wybrano przeprowadzenie operacji konturowanie.\n");
            konturowanie(&obraz);
            break;
          case '5':
            system("clear");
            printf("Wybrano zapisanie obrazu do pliku.\n");
            printf("Podaj nazwe pliku do zapisu: \n");
            scanf("%s", nazwa2);
            plik=fopen(nazwa2, "w");

            if(plik != NULL)
            {
              zapisz(plik, &obraz);
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
  free(obraz.obrazPgm);
  return odczytano;
}
