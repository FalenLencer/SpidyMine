#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include "SpidyLib.h"

bool CheckCollisionWithGrid(Vector2 position, Texture2D playerTexture, Bloc **grille, int rows, int cols, int TailleCarre, int offsetX, int startY, int Espace) {
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


int main(void)
{
    const int initialScreenWidth = 1600;
    const int initialScreenHeight = 900;

    InitWindow(initialScreenWidth, initialScreenHeight, "SpidyMine");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    srand(time(NULL));

    SetTargetFPS(60);

    const int cols = 30;
    const int rows = 20;
    const int Espace = 0;

    const char *blueImagePath = "Nut.png";
    const char *redImagePath = "Minerai1.png";
    const char *yellowImagePath = "Evenement.png";

    Texture2D blueTexture = LoadTextureIfExists(blueImagePath);
    Texture2D redTexture = LoadTextureIfExists(redImagePath);
    Texture2D yellowTexture = LoadTextureIfExists(yellowImagePath);
    Texture2D IconParametreTexture = LoadTextureIfExists("parametre.png");
    Texture2D BackgroundTexture = LoadTextureIfExists("background.png");

    Texture2D playerTextureIdle = LoadTextureIfExists("Mouvement_Perso/neutre.png");
    Texture2D playerTextureMove = LoadTextureIfExists("Mouvement_Perso/bouge 1.png");
    Texture2D playerTextureMoveD = LoadTextureIfExists("Mouvement_Perso/bouge 2.png");
    Texture2D playerTextureAction = LoadTextureIfExists("Mouvement_Perso/mine 1.png");
    Texture2D playerTextureHaut = LoadTextureIfExists("Mouvement_Perso/perso de dos.png");
    Texture2D playerTextureMove2 = LoadTextureIfExists("Mouvement_Perso/perso bouge gauche 1.png");
    Texture2D playerTextureMove2g = LoadTextureIfExists("Mouvement_Perso/perso bouge gauche 2.png");

    Vector2 playerPosition = {(initialScreenWidth/2)-60,(initialScreenHeight/4)-120 };

    int TailleBouton = 80;
    bool ParametreOuvert = false;
    int BoutonMenuWidth = 150;
    int BoutonMenuHeight = 40;
    bool fullscreen = false;

    bool isMovingRight = false;
    bool isMovingLeft = false;
    bool isAction = false;
    bool isMovingBas = false;
    bool isMovingHaut = false;

    int frameCounter = 0;

    Bloc ** Grille=NeedGrid( rows ,  cols , blueTexture, yellowTexture,  redTexture);

    while (!WindowShouldClose())
    {
        int ScreenWidth = GetScreenWidth();
        int ScreenHeight = GetScreenHeight();

        int HauteurLigne = ScreenHeight * 1 / 4;
        int TailleCarreWidth = (ScreenWidth - (cols - 1) * Espace) / cols;
        int TailleCarreHeight = (ScreenHeight - HauteurLigne - (rows - 1) * Espace) / rows;
        int TailleCarre = TailleCarreWidth < TailleCarreHeight ? TailleCarreWidth : TailleCarreHeight;
        int totalWidth = cols * TailleCarre + (cols - 1) * Espace;
        int offsetX = (ScreenWidth - totalWidth) / 2;
        int startY = HauteurLigne + Espace;

        Vector2 PositionBoutonParametre = { ScreenWidth - TailleBouton - 10, 10 };
        Vector2 Pos1600 = { ((ScreenWidth - 400) / 2) + 125, ((ScreenHeight - 300) / 2) + 50 };
        Vector2 PosFull = { ((ScreenWidth - 400) / 2) + 125, ((ScreenHeight - 300) / 2) + 100 };
        Vector2 PosRevenir = { ((ScreenWidth - 400) / 2) + 125, ((ScreenHeight - 300) / 2) + 150 };
        Vector2 PosQuitter = { ((ScreenWidth - 400) / 2) + 125, ((ScreenHeight - 300) / 2) + 200 };
        Vector2 PosSouris=GetMousePosition();

        Rectangle ParaRect = { PositionBoutonParametre.x, PositionBoutonParametre.y, TailleBouton, TailleBouton };
        Rectangle RetourButtonRect = { PosRevenir.x, PosRevenir.y, BoutonMenuWidth, BoutonMenuHeight };
        Rectangle ButtonRect1600 = { Pos1600.x, Pos1600.y, BoutonMenuWidth, BoutonMenuHeight };
        Rectangle FullButtonRect = { PosFull.x, PosFull.y, BoutonMenuWidth, BoutonMenuHeight };
        Rectangle QuitterButtonRect = { PosQuitter.x, PosQuitter.y, BoutonMenuWidth, BoutonMenuHeight };

        isMovingRight = false;
        isMovingLeft = false;
        isAction = false;
        isMovingBas = false;
        isMovingHaut = false;

        if (IsKeyDown(KEY_RIGHT)){
        Vector2 newPosition = playerPosition;
        newPosition.x += 5;
            if (newPosition.x <= ScreenWidth - playerTextureIdle.width ) {
                playerPosition.x += 5;
                isMovingRight = true;
            }
        }

        if (IsKeyDown(KEY_LEFT)){
            Vector2 newPosition = playerPosition;
            newPosition.x -= 5;
            if (newPosition.x >= 0 ) {
                playerPosition.x -= 5;
                isMovingLeft = true;
            }
        }

        if (IsKeyDown(KEY_UP)){
            Vector2 newPosition = playerPosition;
            newPosition.y -= 5;
            if (newPosition.y >= 0 ) {
                playerPosition.y -= 5;
                isMovingHaut = true;
            }
        }

        if (IsKeyDown(KEY_DOWN)){
            Vector2 newPosition = playerPosition;
            newPosition.y += 5;
            if (newPosition.y + playerTextureIdle.height <= ScreenHeight ) {
                playerPosition.y += 5;
                isMovingBas = true;
            }
        }

        if (IsKeyDown(KEY_SPACE)){
            isAction = true;
        }
        frameCounter++;

        BeginDrawing();
        
        ClearBackground(LIGHTGRAY);
        //DrawTexture(BackgroundTexture, 0, 0, WHITE);
        //DrawTexturePro(BackgroundTexture, (Rectangle){ 0, 0, BackgroundTexture.width, BackgroundTexture.height }, (Rectangle){ 0, 0, (float)ScreenWidth, (float)ScreenHeight }, (Vector2){ 0, 0 }, 0.0f, WHITE);

        DrawText(TextFormat("X : %f Y: %f",PosSouris.x,PosSouris.y),0,0,20,BLUE);
        DrawText(TextFormat("offX : %d offY: %d",offsetX,startY),0,20,20,RED);
        DrawLine(0, HauteurLigne, ScreenWidth, HauteurLigne, RED);
        DrawTexture(IconParametreTexture, PositionBoutonParametre.x, PositionBoutonParametre.y, WHITE);

        if (TailleCarre < 1) TailleCarre = 1;

        if (ParametreOuvert) {
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

            if (CheckMouseCollisionCliked(PosSouris,RetourButtonRect)) {
                ParametreOuvert = false;
            }
            if (CheckMouseCollisionCliked(PosSouris,ButtonRect1600)) {
                if (fullscreen==true) {
                    ToggleFullscreen();
                    fullscreen = false;
                }
                ScreenHeight = 900;
                ScreenWidth = 1600;
                SetWindowSize(ScreenWidth, ScreenHeight);
                ParametreOuvert = false;
            }
            if (CheckMouseCollisionCliked(PosSouris,FullButtonRect)) {
                if (fullscreen==true) {
                    ToggleFullscreen();
                    fullscreen = false;
                }
                else{
                    ToggleFullscreen();
                    fullscreen = true;
                    ScreenHeight = GetMonitorHeight(0);
                    ScreenWidth = GetMonitorWidth(0);
                    SetWindowSize(ScreenWidth, ScreenHeight);
                }
                
                ParametreOuvert = false;
            }
            if (CheckMouseCollisionCliked(PosSouris,QuitterButtonRect)) {
                break;
            }
        }
        else if (!ParametreOuvert){
            
            if (CheckMouseCollisionCliked(PosSouris,ParaRect)) {
                ParametreOuvert = true;  
            }

            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    if (VerifEtat(Grille[i][j])) {
                        continue;
                    }
                    int x = offsetX + j * (TailleCarre + Espace);
                    int y = startY + i * (TailleCarre + Espace);

                    DrawTextureEx(Grille[i][j].Texture, (Vector2){x, y}, 0.0f, (float)TailleCarre / Grille[i][j].Texture.width, WHITE);

                    Grille[i][j].HitBox = (Rectangle){x, y, TailleCarre, TailleCarre};
                    
                    if (CheckCollisionPointRec(PosSouris, Grille[i][j].HitBox)) {
                        DrawRectangleLinesEx(Grille[i][j].HitBox, 2, RED);
                        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                            Grille[i][j].Etat = true;
                        }
                    }
                    if (isAction ){
                        DrawTexture(playerTextureAction, playerPosition.x, playerPosition.y, WHITE);
                        if (CheckCollisionPointRec((Vector2) {playerPosition.x+(playerTextureIdle.width)/2,playerPosition.y+playerTextureIdle.height},Grille[i][j].HitBox)){
                            Grille[i][j].Etat=true;
                        }
                    }
                }
            }
            if (IsKeyPressed(KEY_DELETE)) {
                int randomRow = rand() % rows;
                int randomCol = rand() % cols;
                Grille[randomRow][randomCol].Etat = true;
            }
	        else if (isMovingRight){
                if ((frameCounter / 15) % 2 == 0){
                    DrawTexture(playerTextureMove, playerPosition.x, playerPosition.y, WHITE);
                }
                else{
                    DrawTexture(playerTextureMoveD, playerPosition.x, playerPosition.y, WHITE);
                }
            }
            else if (isMovingLeft){
                if ((frameCounter / 15) % 2 == 0){
                    DrawTexture(playerTextureMove2, playerPosition.x, playerPosition.y, WHITE);
                }
                else{
                    DrawTexture(playerTextureMove2g, playerPosition.x, playerPosition.y, WHITE);
                } 
            }
            else if (isMovingBas){
                DrawTexture(playerTextureIdle, playerPosition.x, playerPosition.y, WHITE);
            }
            else if (isMovingHaut){
                DrawTexture(playerTextureHaut, playerPosition.x, playerPosition.y, WHITE);
            }
            else{
                DrawTexture(playerTextureIdle, playerPosition.x, playerPosition.y, WHITE);
            }
        }
        EndDrawing();
    }

    FreeGrid(Grille, rows);

    if (blueTexture.id > 0) UnloadTexture(blueTexture);
    if (redTexture.id > 0) UnloadTexture(redTexture);
    if (yellowTexture.id > 0) UnloadTexture(yellowTexture);
    if (BackgroundTexture.id > 0)UnloadTexture(BackgroundTexture);
    if (IconParametreTexture.id > 0)UnloadTexture(IconParametreTexture);

    UnloadTexture(playerTextureIdle); 
    UnloadTexture(playerTextureMove);
    UnloadTexture(playerTextureMoveD);
    UnloadTexture(playerTextureMove2);
    UnloadTexture(playerTextureMove2g);
    UnloadTexture(playerTextureHaut);
    UnloadTexture(playerTextureAction);

    CloseWindow();
    return 0;
}