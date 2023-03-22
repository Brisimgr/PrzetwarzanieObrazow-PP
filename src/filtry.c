/************************************************************************************
 *Tomasz Lutkowski 263483    	       	       	       	                              *
 *Przetwarzanie obrazow 2                                                           * 
 *Data wykonania: 10.01.22.									                                        *
 ************************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include "../inc/filtry.h"

/************************************************************************************
 * Funkcja przeprowadza operacje negatywu na obrazie PGM lub PPM      	       	    *
 * \param[in] obraz struktura, zawierajaca wszystkie informacje o obrazie           *
 * \param[out] obrazPgm tablica ze zmienionymi przez operacje elementami					  *
 ************************************************************************************/

void negatyw(t_obraz *obraz)
{
  int (*obrazPgm)[obraz->wymX];
  obrazPgm = (int(*)[obraz->wymX]) obraz->obrazPgm;
  /* Podstawienie pod elementy tabeli roznicy skali szarosci i elementu tabeli */
  for(int i = 0; i < obraz->wymY; ++i)
    for(int j = 0; j < obraz->wymX; ++j)
      obrazPgm[i][j] = obraz->odcien - obrazPgm[i][j];

}

/************************************************************************************
 * Funkcja przeprowadza operacja polprogowania bieli na obrazie PGM lub PPM      	  *
 * \param[in] prog wartosc progu, wartosc z przedzialu 0 - 100                      *
 * \param[in] obraz struktura, zawierajaca wszystkie informacje o obrazie		        *
 * \param[out] obrazPgm tablica z ezmienionymi przez operacje elementami			      *
 ************************************************************************************/

void polProgowanieBieli(int prog, t_obraz *obraz)
{
  /* obliczenie progu, na podstawie ktorego bedzie wykoanne progowanie */
  prog = prog/100;
  int polprog = prog*obraz->odcien;

  int (*obrazPgm)[obraz->wymX];
  obrazPgm = (int(*)[obraz->wymX]) obraz->obrazPgm;

  /* zmiana wartosci elementow tabeli, bedacych powyzej podanego progu na maksymalna wartosc skali szarosci */
  for(int i = 0; i < obraz->wymY; ++i)
    for(int j = 0; j < obraz->wymX; ++j)
    {
      if(obrazPgm[i][j] > polprog)
        obrazPgm[i][j] = obraz->odcien;
    }
 
}

/************************************************************************************
 * Funkcja przeprowadza operacje konturowania na obrazie PGM lub PPM      	       	*
 * \param[in] obraz tablica, w kotrej jest przechowywany obraz	                    *		
 * \param[out] obrazPgm tablica ze zmienionymi przez operacje elementami			      *				    					    
 ************************************************************************************/

void konturowanie(t_obraz *obraz)
{
  int (*obrazPgm)[obraz->wymX];
  obrazPgm = (int(*)[obraz->wymX]) obraz->obrazPgm;
  
  int k, l; /* zmienne pomocnicze do obliczenia wartosci pikseli obrazu wynikowego */
  for(int i = 0;i < obraz->wymY; ++i)
    for(int j = 0; j < obraz->wymX; ++j)
    {
      k = obrazPgm[i+1][j] - obrazPgm[i][j];
      l = obrazPgm[i][j+1] - obrazPgm[i][j];
      if(i == 0 && j < obraz->wymX) /* warunek dla pierwszej linijki pikseli */
        obrazPgm[i][j] = abs(l);
      if(i != 0 && j < obraz->wymX) 
        obrazPgm[i][j] = abs(l) + abs(k);
      if(j == obraz->wymX-1) /* warunek dla pikseli skrajnie na prawo */
        obrazPgm[i][j] = abs(k);
      if(i == obraz->wymY-1) /* warunek dla ostatniej linijki pikseli */
        obrazPgm[i][j] = abs(l);
      if(i==obraz->wymY-1 && j == obraz->wymX -1) /* warunek dla piksela w skrajnym prawym, dolnym rogu */
        obrazPgm[i][j] = abs(l) - abs(k);
    }
}

/************************************************************************************
 * Funkcja przeprowadza operacje konwersji obrazu kolorowego na odcienie szarosci   *
 * \param[in] obraz tablica, w kotrej jest przechowywany obraz	                    *		
 * \param[out] obrazPgm tablica ze zmienionymi przez operacje elementami			      *				    					    
 ************************************************************************************/

void konwersja(t_obraz *obraz)
{
  int (*obrazPgm)[obraz->wymX];
  obrazPgm = (int(*)[obraz->wymX]) obraz->obrazPgm;

  int g = 0;

  for(int i = 0; i< obraz->wymY; ++i)
  {
    g = 0;
    for(int j = 0; j< obraz->wymX; j+=3)
    {
      obrazPgm[i][g] = (obrazPgm[i][j] + obrazPgm[i][j+1] + obrazPgm[i][j+2])/3;
      if(obrazPgm[i][g]>obraz->odcien)
        obrazPgm[i][g] = obraz->odcien;
      ++g;
    }
  }
  obraz->jakiObraz = 2;
}

/************************************************************************************
 * Funkcja przeprowadza operacje plotu na obrazie       	       	       	          *
 * \param[in] obraz tablica, w kotrej jest przechowywany obraz                      *
 * \param[in] filtr tablica o rozmiarach 3 na 3, zawierajaca wartosci filtru	      *		
 * \param[out] obrazPgm tablica ze zmienionymi przez operacje elementami			      *				    					    
 ************************************************************************************/

void splot(t_obraz *obraz, int filtr[3][3])
{
  int max = 0, min = 0;
  int (*obrazPgm)[obraz->wymX];
  obrazPgm = (int(*)[obraz->wymX]) obraz->obrazPgm;
  int temp[obraz->wymY][obraz->wymX];

  for(int i = 1; i < obraz->wymY - 1; ++i)
    for(int j = 1; j < obraz->wymX - 1; ++j)
    {
      obrazPgm[i][j] = obrazPgm[i-1][j-1]*filtr[0][0] + obrazPgm[i-1][j]*filtr[0][1] + obrazPgm[i-1][j+1]*filtr[0][2] + obrazPgm[i][j-1]*filtr[1][0] + obrazPgm[i][j]*filtr[1][1] + obrazPgm[i][j+1]*filtr[1][2] + obrazPgm[i+1][j-1]*filtr[2][0] + obrazPgm[i+1][j]*filtr[2][1] + obrazPgm[i+1][j+1]*filtr[2][2];
      
      if(obrazPgm[i][j]>1)
        obrazPgm[i][j] = obrazPgm[i][j]/9;
      if(obrazPgm[i][j]<0)
        obrazPgm[i][j] = 0;
      if(obrazPgm[i][j]>obraz->odcien)
        obrazPgm[i][j] = obraz->odcien;
      
    }
}
  
  /**********************************/
  /* Testowa wersja wzoru na splot  */
  /* ********************************/
  
  /*
  for(int i = 1; i < obraz->wymY - 1; ++i)
    for(int j = 1; j < obraz->wymX - 1; ++j)
    {
      obrazPgm[i][j] = obrazPgm[i-1][j-1]*filtr[0][0] + obrazPgm[i-1][j]*filtr[0][1] + obrazPgm[i-1][j+1]*filtr[0][2] + obrazPgm[i][j-1]*filtr[1][0] + obrazPgm[i][j]*filtr[1][1] + obrazPgm[i][j+1]*filtr[1][2] + obrazPgm[i+1][j-1]*filtr[2][0] + obrazPgm[i+1][j]*filtr[2][1] + obrazPgm[i+1][j+1]*filtr[2][2];
      
      if(obrazPgm[i][j]>1)
        obrazPgm[i][j] = obrazPgm[i][j]/9;
      if(obrazPgm[i][j]<0)
        obrazPgm[i][j] = 0;
      if(obrazPgm[i][j]>obraz->odcien)
        obrazPgm[i][j] = obraz->odcien;
      */
      /*
      if(temp[i][j]>max)
        max = temp[i][j];
      if(temp[i][j]<min)
        min = temp[i][j];
      */  
  /* 
  printf("%d, %d", max, min);

  for(int i = 1; i < obraz->wymY - 1; ++i)
    for(int j = 1; j < obraz->wymX - 1; ++j)
    {
      obrazPgm[i][j] = temp[i][j];
    }
  
  for(int i = 1; i < obraz->wymY - 1; ++i)
    for(int j = 1; j < obraz->wymX - 1; ++j)
    {
      if(obrazPgm[i][j]>0)
        obrazPgm[i][j] = obrazPgm[i][j]/(filtr[0][0]+filtr[0][1]+filtr[0][2]+filtr[1][0]+filtr[1][1]+filtr[1][2]+filtr[2][0]+filtr[2][1]+filtr[2][2]);
      
      if(obrazPgm[i][j]<0)
        obrazPgm[i][j] = ((obrazPgm[i][j] - min) / (max - min)) * obraz->odcien;
      
      if(obrazPgm[i][j]>obraz->odcien)
        obrazPgm[i][j] = obraz->odcien;
      if(obrazPgm[i][j]<0)
        obrazPgm[i][j] = 0;
    }
  */
