#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SpidyLib.h"

Texture2D LoadTextureIfExists(const char *imagePath) {
    if (strlen(imagePath) > 0 && FileExists(imagePath)) {
        return LoadTexture(imagePath);
    }
    printf("Texture bloc introuvable: %s\n", imagePath);
    return (Texture2D){0};
}

int VerifEtat(Bloc B) {
    return (B.Etat == true);
}

Bloc **NeedGrid(int rows, int cols, Texture blueTexture, Texture yellowTexture, Texture redTexture) {
    Bloc **Grille = malloc(sizeof(Bloc*) * rows);

    for (int p = 0; p < rows; p++) {
        Grille[p] = malloc(sizeof(Bloc) * cols);
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int randValue = rand() % 100;
            Grille[i][j].Etat = false; 
            Grille[i][j].PosX = j;
            Grille[i][j].PosY = i;

            if (i < 8) {
                if (randValue < 80) {
                    Grille[i][j].Texture = blueTexture;
                } else if (randValue < 90) {
                    Grille[i][j].Texture = redTexture;
                } else {
                    Grille[i][j].Texture = yellowTexture;
                }
            } else if (i < 14) {
                if (randValue < 60) {
                    Grille[i][j].Texture = blueTexture;
                } else if (randValue < 90) {
                    Grille[i][j].Texture = redTexture;
                } else {
                    Grille[i][j].Texture = yellowTexture;
                }
            } else {
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

bool CheckMouseCollisionCliked(Vector2 PosSouris, Rectangle Rect) {
    return (CheckCollisionPointRec(PosSouris, Rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON));
}

void DrawParametre(int ScreenWidth, int ScreenHeight, Vector2 Pos1600, Vector2 PosFull, Vector2 PosRevenir, Vector2 PosQuitter, int BoutonMenuWidth, int BoutonMenuHeight) {
    DrawRectangle((ScreenWidth - 400) / 2, (ScreenHeight - 300) / 2, 400, 300, LIGHTGRAY);
    DrawText("Paramètre", (ScreenWidth - 400) / 2 + 150, (ScreenHeight - 300) / 2 + 20, 20, BLACK);
    DrawRectangle(Pos1600.x, Pos1600.y, BoutonMenuWidth, BoutonMenuHeight, DARKGRAY);
    DrawText("1600x900", Pos1600.x + 15, Pos1600.y + 10, 20, RAYWHITE);
    DrawRectangle(PosFull.x, PosFull.y, BoutonMenuWidth, BoutonMenuHeight, DARKGRAY);
    DrawText("Plein Écran", PosFull.x + 15, PosFull.y + 10, 20, RAYWHITE);
    DrawRectangle(PosRevenir.x, PosRevenir.y, BoutonMenuWidth, BoutonMenuHeight, DARKGRAY);
    DrawText("Revenir", PosRevenir.x + 15, PosRevenir.y + 10, 20, RAYWHITE);
    DrawRectangle(PosQuitter.x, PosQuitter.y, BoutonMenuWidth, BoutonMenuHeight, RED);
    DrawText("Quitter", PosQuitter.x + 15, PosQuitter.y + 10, 20, RAYWHITE);
}

void ChekCollisionParametre(Vector2 PosSouris, Rectangle RetourButtonRect, Rectangle ButtonRect1600, Rectangle FullButtonRect, Rectangle QuitterButtonRect, bool *ParametreOuvert, bool *fullscreen, int *ScreenWidth, int *ScreenHeight) {
    if (CheckMouseCollisionCliked(PosSouris, RetourButtonRect)) {
        *ParametreOuvert = false;
    }
    if (CheckMouseCollisionCliked(PosSouris, ButtonRect1600)) {
        if (*fullscreen) {
            ToggleFullscreen();
            *fullscreen = false;
        }
        *ScreenHeight = 900;
        *ScreenWidth = 1600;
        SetWindowSize(*ScreenWidth, *ScreenHeight);
        *ParametreOuvert = false;
    }
    if (CheckMouseCollisionCliked(PosSouris, FullButtonRect)) {
        if (*fullscreen) {
            ToggleFullscreen();
            *fullscreen = false;
        } else {
            ToggleFullscreen();
            *fullscreen = true;
            *ScreenHeight = GetMonitorHeight(0);
            *ScreenWidth = GetMonitorWidth(0);
            SetWindowSize(*ScreenWidth, *ScreenHeight);
        }
        *ParametreOuvert = false;
    }
    if (CheckMouseCollisionCliked(PosSouris, QuitterButtonRect)) {
        CloseWindow();
    }
}

void InitTextures(TexturesJeux *textures) {
    (*textures).blueTexture = LoadTextureIfExists("Texture_Blocs/Nut.png");
    (*textures).redTexture = LoadTextureIfExists("Texture_Blocs/Minerai1.png");
    (*textures).yellowTexture = LoadTextureIfExists("Texture_Blocs/Evenement.png");
    (*textures).iconParametreTexture = LoadTextureIfExists("parametre.png");
    (*textures).backgroundTexture = LoadTextureIfExists("background.png");
    (*textures).playerTextureIdle = LoadTextureIfExists("Mouvement_Perso/neutre.png");
    (*textures).playerTextureMove = LoadTextureIfExists("Mouvement_Perso/bouge 1.png");
    (*textures).playerTextureMoveD = LoadTextureIfExists("Mouvement_Perso/bouge 2.png");
    (*textures).playerTextureAction = LoadTextureIfExists("Mouvement_Perso/mine 1.png");
    (*textures).playerTextureHaut = LoadTextureIfExists("Mouvement_Perso/perso de dos.png");
    (*textures).playerTextureMove2 = LoadTextureIfExists("Mouvement_Perso/perso bouge gauche 1.png");
    (*textures).playerTextureMove2g = LoadTextureIfExists("Mouvement_Perso/perso bouge gauche 2.png");
}

void UnloadTextures(TexturesJeux *textures) {
    if ((*textures).blueTexture.id > 0) UnloadTexture((*textures).blueTexture);
    if ((*textures).redTexture.id > 0) UnloadTexture((*textures).redTexture);
    if ((*textures).yellowTexture.id > 0) UnloadTexture((*textures).yellowTexture);
    if ((*textures).iconParametreTexture.id > 0) UnloadTexture((*textures).iconParametreTexture);
    if ((*textures).backgroundTexture.id > 0) UnloadTexture((*textures).backgroundTexture);
    if ((*textures).playerTextureIdle.id > 0) UnloadTexture((*textures).playerTextureIdle);
    if ((*textures).playerTextureMove.id > 0) UnloadTexture((*textures).playerTextureMove);
    if ((*textures).playerTextureMoveD.id > 0) UnloadTexture((*textures).playerTextureMoveD);
    if ((*textures).playerTextureAction.id > 0) UnloadTexture((*textures).playerTextureAction);
    if ((*textures).playerTextureHaut.id > 0) UnloadTexture((*textures).playerTextureHaut);
    if ((*textures).playerTextureMove2.id > 0) UnloadTexture((*textures).playerTextureMove2);
    if ((*textures).playerTextureMove2g.id > 0) UnloadTexture((*textures).playerTextureMove2g);
}

void GetMouvements(int Speed, int ScreenWidth, int ScreenHeight, bool *isAction, bool *isMovingRight, bool *isMovingLeft, bool *isMovingHaut, bool *isMovingBas, Vector2 *playerPosition, TexturesJeux textures) {
    if (IsKeyDown(KEY_SPACE)) {
        *isAction = true;
    } else {
        if (IsKeyDown(KEY_RIGHT)) {
            Vector2 newPosition = *playerPosition;
            newPosition.x += Speed;
            if (newPosition.x <= ScreenWidth - textures.playerTextureIdle.width) {
                (*playerPosition).x += Speed;
                *isMovingRight = true;
            }
        }
        if (IsKeyDown(KEY_LEFT)) {
            Vector2 newPosition = *playerPosition;
            newPosition.x -= Speed;
            if (newPosition.x >= 0) {
                (*playerPosition).x -= Speed;
                *isMovingLeft = true;
            }
        }
        if (IsKeyDown(KEY_UP)) {
            Vector2 newPosition = *playerPosition;
            newPosition.y -= Speed;
            if (newPosition.y >= 0) {
                (*playerPosition).y -= Speed;
                *isMovingHaut = true;
            }
        }
        if (IsKeyDown(KEY_DOWN)) {
            Vector2 newPosition = *playerPosition;
            newPosition.y += Speed;
            if (newPosition.y + textures.playerTextureIdle.height <= ScreenHeight) {
                (*playerPosition).y += Speed;
                *isMovingBas = true;
            }
        }
    }
}

void DrawMouvements(bool isAction,bool isMovingRight,bool isMovingLeft ,bool isMovingBas,bool isMovingHaut, int frameCounter,Vector2 playerPosition ,TexturesJeux textures, float echelle){
    if (isAction) {
        DrawTextureEx(textures.playerTextureAction, playerPosition,0.0,echelle, WHITE);
        return;
    }
    if (isMovingRight) {
        if ((frameCounter / 15) % 2 == 0) {
            DrawTextureEx(textures.playerTextureMove, playerPosition, 0.0, echelle, WHITE);
        } else {
            DrawTextureEx(textures.playerTextureMoveD, playerPosition, 0.0, echelle, WHITE);
        }
    }
    else if (isMovingLeft) {
        if ((frameCounter / 15) % 2 == 0) {
            DrawTextureEx(textures.playerTextureMove2, playerPosition, 0.0, echelle, WHITE);
        } else {
            DrawTextureEx(textures.playerTextureMove2g, playerPosition, 0.0, echelle, WHITE);
        } 
    }
    else if (isMovingBas) {
        DrawTextureEx(textures.playerTextureIdle, playerPosition, 0.0, echelle, WHITE);
    }
    else if (isMovingHaut) {
        DrawTextureEx(textures.playerTextureHaut, playerPosition, 0.0, echelle, WHITE);
    }
    else {
        DrawTextureEx(textures.playerTextureIdle, playerPosition, 0.0, echelle, WHITE);
    }
}

void SuprCliked(Vector2 PosSouris , Bloc *cube){
    if (CheckCollisionPointRec(PosSouris, (*cube).HitBox)) {
        DrawRectangleLinesEx((*cube).HitBox, 2, RED);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            (*cube).Etat = true;
        }
    }
}
void CheckOuvertureParametre( Vector2 PosSouris, Rectangle ParaRect ,bool *ParametreOuvert){
    if (CheckMouseCollisionCliked(PosSouris, ParaRect)) {
        *ParametreOuvert = true;  
    }
}

void DetecterCollision(Rectangle Personage, Bloc *Cube){
    DrawRectangleLines(Personage.x,Personage.y,Personage.width,Personage.height,ORANGE);
    if(CheckCollisionRecs((*Cube).HitBox,Personage)){
        DrawRectangleLinesEx((*Cube).HitBox, 2, BLUE);
    }
}