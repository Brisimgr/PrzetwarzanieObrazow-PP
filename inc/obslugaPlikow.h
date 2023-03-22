#ifndef OBSLUGAPLIKOW_H
#define OBSLUGAPLIKOW_H
#define MAX 512            /* Maksymalny rozmiar wczytywanego obrazu */
#define DL_LINII 1024  

#include "struktury.h"

int czytaj(FILE *, t_obraz *);
void zapisz(FILE *, t_obraz *);
void wyswietl(char *);

#endif
