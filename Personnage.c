#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SpidyLib.h"
#include "CalculLib.h"

// Fonction pour vérifier si un personnage entre en collision avec un bloc de la grille
bool IsCollidingWithBloc(Rectangle personnage, Bloc ***Grille, int rows, int cols, int additionalCols) {
    bool colliding = false; // Variable pour détecter la collision
    // Parcours de la grille pour vérifier la collision
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols + 2 * additionalCols; j++) {
            // Si le bloc n'est pas miné et qu'il y a une collision, on permet de miner ce bloc
            if (!(*Grille)[i][j].Etat && CheckCollisionRecs(personnage, (*Grille)[i][j].HitBox)) {
                (*Grille)[i][j].PeutMiner = true;
                colliding = true;
            }
            else {
                (*Grille)[i][j].PeutMiner = false;
            }
        }
    }
    return colliding; // Retourne si une collision a été détectée
}

// Fonction de débogage pour dessiner la collision entre le personnage et un bloc
void DetecterCollision(Rectangle Personage, Bloc *Cube) {
    DrawRectangleLines(Personage.x, Personage.y, Personage.width, Personage.height, ORANGE);
    // Si collision ou le bloc peut être miné, dessine les contours du bloc en bleu
    if (CheckCollisionRecs((*Cube).HitBox, Personage) || Cube->PeutMiner) {
        DrawRectangleLinesEx((*Cube).HitBox, 2, BLUE);
    }
}

// Fonction pour gérer les mouvements du joueur
void GetMouvements(
    int Speed, int ScreenWidth, int ScreenHeight, bool *isAction, bool *isMovingRight, bool *isMovingLeft,
    bool *isMovingHaut, bool *isMovingBas, Vector2 *playerPosition, TexturesJeux textures,
    Bloc ***Grille, int rows, int cols, int additionalCols, float echelle
) {
    Rectangle personnage = (Rectangle) { (*playerPosition).x, (*playerPosition).y, (textures.playerTextureIdle.width) * echelle, (textures.playerTextureIdle.height) * echelle };

    // Si le joueur clique avec le bouton gauche, il est en action
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        *isAction = true;
    } else {
        Vector2 newPosition = *playerPosition;

        // Vérification des mouvements dans chaque direction
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
            newPosition.x += Speed; // Déplacement vers la droite
            personnage.x = newPosition.x;
            // Si pas de collision, met à jour la position du joueur
            if (newPosition.x <= ScreenWidth - textures.playerTextureIdle.width && !IsCollidingWithBloc(personnage, Grille, rows, cols, additionalCols)) {
                (*playerPosition).x = newPosition.x;
                *isMovingRight = true;
            }
        }
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
            newPosition.x -= Speed; // Déplacement vers la gauche
            personnage.x = newPosition.x;
            if (newPosition.x >= 0 && !IsCollidingWithBloc(personnage, Grille, rows, cols, additionalCols)) {
                (*playerPosition).x = newPosition.x;
                *isMovingLeft = true;
            }
        }
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
            newPosition.y -= Speed; // Déplacement vers le haut
            personnage.y = newPosition.y;
            if (newPosition.y >= 0 && !IsCollidingWithBloc(personnage, Grille, rows, cols, additionalCols)) {
                (*playerPosition).y = newPosition.y;
                *isMovingHaut = true;
            }
        }
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
            newPosition.y += Speed; // Déplacement vers le bas
            personnage.y = newPosition.y;
            if (newPosition.y + textures.playerTextureIdle.height <= ScreenHeight && !IsCollidingWithBloc(personnage, Grille, rows, cols, additionalCols)) {
                (*playerPosition).y = newPosition.y;
                *isMovingBas = true;
            }
        }
    }
}

// Fonction pour dessiner les mouvements du joueur en fonction de son état
void DrawMouvements(
    bool isAction, bool isMovingRight, bool isMovingLeft, bool isMovingBas, bool isMovingHaut,
    int frameCounter, Vector2 playerPosition, TexturesJeux textures, float echelle
) {
    // Si le joueur est en action, alterne entre deux textures d'action
    if (isAction) {
        if ((frameCounter / 15) % 2 == 0) {
            DrawTextureEx(textures.playerTextureAction, playerPosition, 0.0, echelle, WHITE);
        } else {
            DrawTextureEx(textures.playerTextureAction2, playerPosition, 0.0, echelle, WHITE);
        }
        return;
    }

    // Dessine les textures de mouvement en fonction de la direction
    if (isMovingRight) {
        if ((frameCounter / 15) % 2 == 0) {
            DrawTextureEx(textures.playerTextureMoveDroite1, playerPosition, 0.0, echelle, WHITE);
        } else {
            DrawTextureEx(textures.playerTextureMoveDroite2, playerPosition, 0.0, echelle, WHITE);
        }
    }
    else if (isMovingLeft) {
        if ((frameCounter / 15) % 2 == 0) {
            DrawTextureEx(textures.playerTextureMoveGauche1, playerPosition, 0.0, echelle, WHITE);
        } else {
            DrawTextureEx(textures.playerTextureMoveGauche2, playerPosition, 0.0, echelle, WHITE);
        } 
    }
    else if (isMovingBas) {
        if ((frameCounter / 15) % 2 == 0) {
            DrawTextureEx(textures.playerTextureBas, playerPosition, 0.0, echelle, WHITE);
        } else {
            DrawTextureEx(textures.playerTextureBas2, playerPosition, 0.0, echelle, WHITE);
        } 
    }
    else if (isMovingHaut) {
        if ((frameCounter / 15) % 2 == 0) {
            DrawTextureEx(textures.playerTextureHaut, playerPosition, 0.0, echelle, WHITE);
        } else {
            DrawTextureEx(textures.playerTextureHaut2, playerPosition, 0.0, echelle, WHITE);
        } 
    }
    else {
        // Si le joueur ne bouge pas, affiche la texture de repos
        DrawTextureEx(textures.playerTextureIdle, playerPosition, 0.0, echelle, WHITE);
    }
}
