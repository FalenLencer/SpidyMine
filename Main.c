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
    Texture2D IconParametreTexture = LoadTextureIfExists("parametre.png");
    Texture2D BackgroundTexture = LoadTextureIfExists("background.png");

    int TailleBouton = 80;
    bool ParametreOuvert = false;
    int BoutonMenuWidth = 150;
    int BoutonMenuHeight = 40;
    bool fullscreen = false;

    Bloc ** Grille=NeedGrid( rows ,  cols , blueTexture, yellowTexture,  redTexture);

    while (!WindowShouldClose())
    {
        int ScreenWidth = GetScreenWidth();
        int ScreenHeight = GetScreenHeight();

        Vector2 PositionBoutonParametre = { ScreenWidth - TailleBouton - 10, 10 };
        Vector2 Pos1600 = { ((ScreenWidth - 400) / 2) + 125, ((ScreenHeight - 300) / 2) + 50 };
        Vector2 PosFull = { ((ScreenWidth - 400) / 2) + 125, ((ScreenHeight - 300) / 2) + 100 };
        Vector2 PosRevenir = { ((ScreenWidth - 400) / 2) + 125, ((ScreenHeight - 300) / 2) + 150 };
        Vector2 PosQuitter = { ((ScreenWidth - 400) / 2) + 125, ((ScreenHeight - 300) / 2) + 200 };

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePosition = GetMousePosition();

            if (!ParametreOuvert) {
                Rectangle ParaRect = { PositionBoutonParametre.x, PositionBoutonParametre.y, TailleBouton, TailleBouton };
                if (CheckCollisionPointRec(mousePosition, ParaRect)) {
                    ParametreOuvert = true;  
                }
            } else {
                Rectangle RetourButtonRect = { PosRevenir.x, PosRevenir.y, BoutonMenuWidth, BoutonMenuHeight };
                if (CheckCollisionPointRec(mousePosition, RetourButtonRect)) {
                    ParametreOuvert = false;
                }
                Rectangle ButtonRect1600 = { Pos1600.x, Pos1600.y, BoutonMenuWidth, BoutonMenuHeight };
                if (CheckCollisionPointRec(mousePosition, ButtonRect1600)) {
                    if (fullscreen) {
                        ToggleFullscreen();
                        fullscreen = false;
                    }
                    ScreenHeight = 900;
                    ScreenWidth = 1600;
                    SetWindowSize(ScreenWidth, ScreenHeight);
                    ParametreOuvert = false;
                }
                Rectangle FullButtonRect = { PosFull.x, PosFull.y, BoutonMenuWidth, BoutonMenuHeight };
                if (CheckCollisionPointRec(mousePosition, FullButtonRect)) {
                    ToggleFullscreen();
                    fullscreen = true;
                    ScreenHeight = GetMonitorHeight(0);
                    ScreenWidth = GetMonitorWidth(0);
                    SetWindowSize(ScreenWidth, ScreenHeight);
                    ParametreOuvert = false;
                }

                Rectangle QuitterButtonRect = { PosQuitter.x, PosQuitter.y, BoutonMenuWidth, BoutonMenuHeight };
                if (CheckCollisionPointRec(mousePosition, QuitterButtonRect)) {
                    break;
                }
            }
        }

        int HauteurLigne = ScreenHeight * 1 / 4;

        int TailleCarreWidth = (ScreenWidth - (cols - 1) * Espace) / cols;
        int TailleCarreHeight = (ScreenHeight - HauteurLigne - (rows - 1) * Espace) / rows;
        int TailleCarre = TailleCarreWidth < TailleCarreHeight ? TailleCarreWidth : TailleCarreHeight;

        int totalWidth = cols * TailleCarre + (cols - 1) * Espace;
        int offsetX = (ScreenWidth - totalWidth) / 2;
        int startY = HauteurLigne + Espace;

        Vector2 PosSouris=GetMousePosition();

        if (IsKeyPressed(KEY_DELETE)) {
            int randomRow = rand() % rows;
            int randomCol = rand() % cols;
            Grille[randomRow][randomCol].Etat = true;
        }
        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        DrawTexture(BackgroundTexture, 0, 0, WHITE);
        DrawTexturePro(BackgroundTexture, (Rectangle){ 0, 0, BackgroundTexture.width, BackgroundTexture.height }, (Rectangle){ 0, 0, (float)ScreenWidth, (float)ScreenHeight }, (Vector2){ 0, 0 }, 0.0f, WHITE);

        DrawText(TextFormat("X : %f Y: %f",PosSouris.x,PosSouris.y),0,0,20,BLUE);
        DrawText(TextFormat("offX : %d offY: %d",offsetX,startY),0,20,20,RED);
        DrawLine(0, HauteurLigne, ScreenWidth, HauteurLigne, RED);

        if (TailleCarre < 1) TailleCarre = 1;
        
        if (!ParametreOuvert){
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
                }
            }
        }

        DrawTexture(IconParametreTexture, PositionBoutonParametre.x, PositionBoutonParametre.y, WHITE);

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
        }
        EndDrawing();
    }

    FreeGrid(Grille, rows);

    if (blueTexture.id > 0) UnloadTexture(blueTexture);
    if (redTexture.id > 0) UnloadTexture(redTexture);
    if (yellowTexture.id > 0) UnloadTexture(yellowTexture);
    if (BackgroundTexture.id > 0)UnloadTexture(BackgroundTexture);
    if (IconParametreTexture.id > 0)UnloadTexture(IconParametreTexture);
    
    CloseWindow();

    return 0;
}