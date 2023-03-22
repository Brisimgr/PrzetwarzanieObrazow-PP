#include<stdio.h>
#include<stdlib.h>
#include "../inc/obslugaPlikow.h"
#include "../inc/filtry.h"
#include "../inc/opcje.h"

int czyPoprawne = 0;

int main(int argc, char ** argv) {
  //t_obraz obraz;
  t_opcje opcje;
  int kodBledu;
  int odczytano = 0;
  
  kodBledu = przetwarzaj_opcje(argc,argv,&opcje);
  /*
  if(kodBledu)
    printf("Blad nr %d.\n", kodBledu);
  else  
    {
      
      if (opcje->plik_we != NULL) 
      {       
        odczytano = czytaj(opcje->plik_we, &obraz);
        fclose(opcje->plik_we);

        if(opcje->negatyw == 1)
        {
          if(obraz.jakiObraz==0)
              konwersja(&obraz);
          negatyw(&obraz);
        }
        if(opcje->konturowanie == 1)
        {
          if(obraz.jakiObraz==0)
              konwersja(&obraz);
          konturowanie(&obraz);
        }
        if(opcje->progowanie == 1)
        {
          if(obraz.jakiObraz==0)
              konwersja(&obraz);
          polProgowanieBieli(opcje->w_progu, &obraz);
        }
        if(opcje->wyswietlenie == 1)
        {
          if(odczytano != 0)
            wyswietl(opcje->nazwaPlikuWe);
        }
      }
    }
  */
  if (kodBledu)
    printf("Blad nr %d\n", kodBledu);
  else
    printf("Opcje poprawne\n");

  //free(obraz.obrazPgm);
  printf("%s %s", argv[1], argv[2]);
  return odczytano;
}