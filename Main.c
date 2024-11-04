#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include "SpidyLib.h"

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

    TexturesJeux textures;  
    InitTextures(&textures);

    Vector2 playerPosition = {(initialScreenWidth / 2) - 60, (initialScreenHeight / 4) - 120 };

    int prevScreenWidth = initialScreenWidth;
    int prevScreenHeight = initialScreenHeight;

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

    Bloc **Grille = NeedGrid(rows, cols, textures.blueTexture, textures.yellowTexture, textures.redTexture);

    int Speed = 2;

    while (!WindowShouldClose()){

        int ScreenWidth = GetScreenWidth();
        int ScreenHeight = GetScreenHeight();

       if (ScreenWidth != prevScreenWidth || ScreenHeight != prevScreenHeight) {
        float playerXPercent = (playerPosition).x / (float)(prevScreenWidth);
        float playerYPercent = (playerPosition).y / (float)(prevScreenHeight);

        (playerPosition).x = playerXPercent * ScreenWidth;
        (playerPosition).y = playerYPercent * ScreenHeight;

        (prevScreenWidth) = ScreenWidth;
        (prevScreenHeight) = ScreenHeight;
        }
        
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
        Vector2 PosSouris = GetMousePosition();

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

        GetMouvements(Speed, ScreenWidth, ScreenHeight, &isAction, &isMovingRight, &isMovingLeft, &isMovingHaut, &isMovingBas, &playerPosition, textures);

        frameCounter++;

        BeginDrawing();
        
        ClearBackground(LIGHTGRAY);

        DrawLine(0, HauteurLigne, ScreenWidth, HauteurLigne, RED);
        DrawTexture(textures.iconParametreTexture, PositionBoutonParametre.x, PositionBoutonParametre.y, WHITE);

        if (TailleCarre < 1) TailleCarre = 1;

        if (ParametreOuvert) {
            
            DrawParametre(ScreenWidth, ScreenHeight, Pos1600, PosFull, PosRevenir, PosQuitter, BoutonMenuWidth, BoutonMenuHeight);
            ChekCollisionParametre(PosSouris, RetourButtonRect, ButtonRect1600, FullButtonRect, QuitterButtonRect, &ParametreOuvert, &fullscreen, &ScreenWidth, &ScreenHeight);
        }
        else if (!ParametreOuvert) {
            CheckOuvertureParametre(  PosSouris,  ParaRect , &ParametreOuvert);
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

                    SuprCliked( PosSouris , &Grille[i][j]);

                    ActionMiner( isAction , textures ,  playerPosition ,  &Grille[i][j]);
                }
            }
            DrawMouvements( isMovingRight, isMovingLeft , isMovingBas, isMovingHaut,  frameCounter, playerPosition , textures);
        }
        EndDrawing();
    }
    UnloadTextures(&textures);
    FreeGrid(Grille, rows);
    CloseWindow();
    return 0;
}