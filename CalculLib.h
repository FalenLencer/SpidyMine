#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void echanger(int* a, int* b) ;

int partitionner(int tableau[], int bas, int haut);
void triRapide(int tableau[], int bas, int haut);

int partitionnerInverse(int tableau[], int bas, int haut) ;
void triRapideInverse(int tableau[], int bas, int haut);

void genererListe(int liste[], int taille, int Pourcentage) ;

int* GetPourcentageCroissant(int Taille, int Pourcentage)  ;

int* GetPourcentageDecroissant(int Taille, int Pourcentage) ;

int* GetPourcentagePick(int Taille, int Pourcentge );