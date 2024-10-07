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
    const int Espace = 5;

    const char *blueImagePath = "Nut.png";
    const char *redImagePath = "Minerai1.png";
    const char *yellowImagePath = "Evenement.png";

    Texture2D blueTexture = LoadTextureIfExists(blueImagePath);
    Texture2D redTexture = LoadTextureIfExists(redImagePath);
    Texture2D yellowTexture = LoadTextureIfExists(yellowImagePath);

    Texture2D background = LoadTexture("background.png");

    Bloc ** Grille=NeedGrid( rows ,  cols , blueTexture, yellowTexture,  redTexture);

    while (!WindowShouldClose())
    {
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        int HauteurLigne = screenHeight * 1 / 4;

        int TailleCarreWidth = (screenWidth - (cols - 1) * Espace) / cols;
        int TailleCarreHeight = (screenHeight - HauteurLigne - (rows - 1) * Espace) / rows;
        int TailleCarre = TailleCarreWidth < TailleCarreHeight ? TailleCarreWidth : TailleCarreHeight;

        int totalWidth = cols * TailleCarre + (cols - 1) * Espace;
        int offsetX = (screenWidth - totalWidth) / 2;
        int startY = HauteurLigne + Espace;

        Vector2 PosSouris=GetMousePosition();

        
        if (IsKeyPressed(KEY_DELETE)) {
            int randomRow = rand() % rows;
            int randomCol = rand() % cols;
            Grille[randomRow][randomCol].Etat = true;
        }
        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        DrawTexture(background, 0, 0, WHITE);
        DrawText(TextFormat("X : %f Y: %f",PosSouris.x,PosSouris.y),0,0,20,BLUE);
        DrawText(TextFormat("offX : %d offY: %d",offsetX,startY),0,20,20,RED);
        DrawLine(0, HauteurLigne, screenWidth, HauteurLigne, RED);

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
            }
        }
        EndDrawing();
    }

    if (blueTexture.id > 0) UnloadTexture(blueTexture);
    if (redTexture.id > 0) UnloadTexture(redTexture);
    if (yellowTexture.id > 0) UnloadTexture(yellowTexture);

    CloseWindow();

    return 0;
}