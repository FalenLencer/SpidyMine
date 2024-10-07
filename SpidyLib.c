#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Bloc
{
   Texture2D Texture;
   int PosX;
   int PosY;
   bool Etat;
   int Type;
}Bloc;

Texture2D LoadTextureIfExists(const char *imagePath) {
    if (strlen(imagePath) > 0 && FileExists(imagePath)) {
        return LoadTexture(imagePath);
    }
    printf("texture bloc introuvable: %s\n", imagePath);
    return (Texture2D){0};
}

int VerifEtat(Bloc B) {
    return (B.Etat==true);
}

Bloc ** NeedGrid(int rows , int cols ,Texture blueTexture,Texture yellowTexture, Texture redTexture){

    Bloc ** Grille=malloc(sizeof(Bloc*)*rows);

    for (int p=0;p<cols;p++){
        Grille[p]=malloc(sizeof(Bloc)*cols);
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int randValue = rand() % 100;
            Grille[i][j].Etat=false; 
            Grille[i][j].PosX=j;
            Grille[i][j].PosY=i;

            if (i<8){
                if (randValue < 80) {
                    Grille[i][j].Texture = blueTexture;
                } else if (randValue < 90) {
                    Grille[i][j].Texture = redTexture;
                } else {
                    Grille[i][j].Texture = yellowTexture;
                }
            }
            else if (i<14){
                if (randValue < 60) {
                    Grille[i][j].Texture = blueTexture;
                } else if (randValue < 90) {
                    Grille[i][j].Texture = redTexture;
                } else {
                    Grille[i][j].Texture = yellowTexture;
                }
            }
            else{
                if (randValue < 40) {
                    Grille[i][j].Texture = blueTexture;
                } else if (randValue < 90) {
                    Grille[i][j].Texture = redTexture;
                } else {
                    Grille[i][j].Texture = yellowTexture;
                }
            }
        }
    }
    return Grille;
}