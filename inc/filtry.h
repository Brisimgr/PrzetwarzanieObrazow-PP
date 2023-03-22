#ifndef FILTRY_H
#define FILTRY_H
#define MAX 512            /* Maksymalny rozmiar wczytywanego obrazu */
#define DL_LINII 1024

#include "struktury.h"

void negatyw(t_obraz *);
void polProgowanieBieli(int , t_obraz *);
void konturowanie(t_obraz *);
void konwersja(t_obraz *);
void splot(t_obraz *, int [3][3]);

#endif
