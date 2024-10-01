#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

Texture2D LoadTextureIfExists(const char *imagePath) {
    if (strlen(imagePath) > 0 && FileExists(imagePath)) {
        return LoadTexture(imagePath);
    }
    printf("texture bloc introuvable: %s\n", imagePath);
    return (Texture2D){0};
}

int CompareCouleur(Color a, Color b) {
    return (a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a);
}

int main(void)
{
    const int initialScreenWidth = 1600;
    const int initialScreenHeight = 900;

    InitWindow(initialScreenWidth, initialScreenHeight, "Jeu");
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

    Color colors[rows][cols];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int randValue = rand() % 100;

            if (randValue < 50) {
                colors[i][j] = BLUE;
            } else if (randValue < 90) {
                colors[i][j] = RED;
            } else {
                colors[i][j] = YELLOW;
            }
        }
    }

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

        if (IsKeyPressed(KEY_DELETE)) {
            int randomRow = rand() % rows;
            int randomCol = rand() % cols;
            colors[randomRow][randomCol] = BLANK;
        }
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawTexture(background, 0, 0, WHITE);

        DrawLine(0, HauteurLigne, screenWidth, HauteurLigne, RED);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (CompareCouleur(colors[i][j], BLANK)) {
                    continue;
                }

                int x = offsetX + j * (TailleCarre + Espace);
                int y = startY + i * (TailleCarre + Espace);

                if (CompareCouleur(colors[i][j], BLUE) && blueTexture.id > 0) {
                    DrawTextureEx(blueTexture, (Vector2){x, y}, 0.0f, (float)TailleCarre / blueTexture.width, WHITE);
                } else if (CompareCouleur(colors[i][j], RED) && redTexture.id > 0) {
                    DrawTextureEx(redTexture, (Vector2){x, y}, 0.0f, (float)TailleCarre / redTexture.width, WHITE);
                } else if (CompareCouleur(colors[i][j], YELLOW) && yellowTexture.id > 0) {
                    DrawTextureEx(yellowTexture, (Vector2){x, y}, 0.0f, (float)TailleCarre / yellowTexture.width, WHITE);
                } else {
                    DrawRectangle(x, y, TailleCarre, TailleCarre, colors[i][j]);
                }
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