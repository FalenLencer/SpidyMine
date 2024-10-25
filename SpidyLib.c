#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SpidyLib.h"

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

    for (int p=0;p<rows;p++){
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
            Grille[i][j].HitBox = (Rectangle){0, 0, 0, 0};
        }
    }
    return Grille;
}

void FreeGrid(Bloc **Grille, int rows) {
    for (int i = 0; i < rows; i++) {
        free(Grille[i]);
    }
    free(Grille);
}

bool CheckMouseCollisionCliked(Vector2 PosSouris,Rectangle Rect){
    return ( (CheckCollisionPointRec(PosSouris, Rect)) && (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)));
}

/*bool CheckCollisionWithGrid(Vector2 position, Texture2D playerTexture, Bloc **grille, int rows, int cols, int TailleCarre, int offsetX, int startY, int Espace) {
    // Calculer le centre du joueur (bas du joueur pour la collision)
    Vector2 playerCenter = {position.x + playerTexture.width / 2, position.y + playerTexture.height};

    // Parcourir la grille et vérifier les collisions avec les blocs non traversables
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (VerifEtat(grille[i][j])) { // Si le bloc est activé ou déjà détruit, passer.
                continue;
            }

            // Calculer la position de chaque bloc
            int x = offsetX + j * (TailleCarre + Espace);
            int y = startY + i * (TailleCarre + Espace);

            // Créer une hitbox pour chaque bloc
            Rectangle blocHitbox = {x, y, TailleCarre, TailleCarre};

            // Vérifier la collision entre le centre du joueur et la hitbox du bloc
            if (CheckCollisionPointRec(playerCenter, blocHitbox)) {
                return true; // Collision détectée
            }
        }
    }
    return false; // Pas de collision
}
*/