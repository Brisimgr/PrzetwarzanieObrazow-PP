#ifndef STRUKTURY_H
#define STRUKTURY_H
#define MAX 512   

/* typ do przechowywania obrazów monochoromatycznych */
typedef struct obraz{
int wymX, wymY, odcien;
void *obrazPgm;
int jakiObraz;  /* Od wartosci zalezy jaki format obrazu obecnie obslugujemy: 0 - format PPM, bez konwersji; 1 - format PGM, bez konwersji; 2 - format PGM, po konwersji z PPM */ 
} t_obraz;

/* strukura do zapamietywania opcji podanych w wywolaniu programu */
typedef struct opcje{
  FILE *plik_we, *plik_wy;        /* uchwyty do pliku wej. i wyj. */
  int negatyw,progowanie,konturowanie,wyswietlenie, splot;      /* opcje */
  int w_progu;              /* wartosc progu dla opcji progowanie */ 
} t_opcje;

#endif
