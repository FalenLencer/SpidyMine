#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "CalculLib.h"

void echanger(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partitionner(int tableau[], int bas, int haut) {
    int pivot = tableau[haut];
    int i = bas - 1;

    for (int j = bas; j < haut; j++) {
        if (tableau[j] <= pivot) {
            i++;
            echanger(&tableau[i], &tableau[j]);
        }
    }
    echanger(&tableau[i + 1], &tableau[haut]);
    return i + 1;
}

void triRapide(int tableau[], int bas, int haut) {
    if (bas < haut) {
        int pivotIndex = partitionner(tableau, bas, haut);

        triRapide(tableau, bas, pivotIndex - 1);
        triRapide(tableau, pivotIndex + 1, haut);
    }
}

int partitionnerInverse(int tableau[], int bas, int haut) {
    int pivot = tableau[haut];
    int i = bas - 1;

    for (int j = bas; j < haut; j++) {
        if (tableau[j] >= pivot) {
            i++;
            echanger(&tableau[i], &tableau[j]);
        }
    }
    echanger(&tableau[i + 1], &tableau[haut]);
    return i + 1;
}

void triRapideInverse(int tableau[], int bas, int haut) {
    if (bas < haut) {
        int pivotIndex = partitionnerInverse(tableau, bas, haut);

        triRapideInverse(tableau, bas, pivotIndex - 1);
        triRapideInverse(tableau, pivotIndex + 1, haut);
    }
}

void genererListe(int liste[], int taille , int Pourcentage) {
    int somme = 0;

    srand(time(NULL));

    liste[0] = 3*Pourcentage/5 + rand() % ((2*Pourcentage/5)+1); 
    somme += liste[0];

    for (int i = 1; i < taille; i++) {
        int reste = Pourcentage - somme - (taille - i - 1);
        if (reste <= 0) {
            reste = 1;
        }
        liste[i] = rand() % (reste + 1);
        somme += liste[i];
    }

    if (liste[taille - 1] <= 0) {
        liste[taille - 1] = 1;
        somme += 1;
    }

    int difference = Pourcentage - somme;
    if (difference != 0) {
        liste[0] += difference;
        somme += difference; 
    }
}

void afficherListe(int liste[], int taille) {
    for (int i = 0; i < taille; i++) {
        printf("%d ", liste[i]);
    }
    printf("\n");
}

int* GetPourcentageCroissant(int Taille , int Pourcentage) {
    int *liste = (int*)malloc(Taille * sizeof(int));

    if (liste == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    genererListe(liste, Taille,Pourcentage);
    triRapide(liste, 0, Taille - 1);
    return liste;
}

int* GetPourcentageDecroissant(int Taille, int Pourcentage) {
    int *liste = (int*)malloc(Taille * sizeof(int));

    if (liste == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    genererListe(liste, Taille, Pourcentage);
    triRapideInverse(liste, 0, Taille - 1);
    return liste;
}

int* GetPourcentagePick(int Taille, int Poucentage ) {
    int *liste = (int*)malloc(Taille * sizeof(int));
    if (liste == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    
    int moitie = Taille / 2;
    int reste = Taille - moitie;

    int PoG=Poucentage/2;
    int PoD= Poucentage-PoG;
    int *Gauche=GetPourcentageCroissant(moitie ,PoG);
    int *Droite=GetPourcentageDecroissant(reste, PoD);
    
    for (int i=0; i<moitie;i++){
        liste[i]=Gauche[i];
    }
    for (int i=0; i<reste;i++){
        liste[moitie+i]=Droite[i];
    }
    free(Gauche);
    free(Droite);

    return liste;
}
/*
int main() {
    int taille = 8;
    int Pourcentage = 100;
    int *listeCroissante = GetPourcentageCroissant(taille ,Pourcentage);
    printf("Liste triée en ordre croissant :\n");
    afficherListe(listeCroissante, taille);
    free(listeCroissante);

    int *listeDecroissante = GetPourcentageDecroissant(taille, Pourcentage);
    printf("Liste triée en ordre décroissant :\n");
    afficherListe(listeDecroissante, taille);
    free(listeDecroissante);

    int *listePick = GetPourcentagePick(taille);
    printf("Liste triée en ordre Pick :\n");
    afficherListe(listePick, taille);
    free(listePick);

    return 0;
}

*/
