#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include "SpidyLib.h"
#include "CalculLib.h"

int main(void)
{
    const int initialScreenWidth = 1600;
    const int initialScreenHeight = 900;

    InitWindow(initialScreenWidth, initialScreenHeight, "SpidyMine");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    
    srand(time(NULL));
    SetTargetFPS(60);

    const int cols = 15;
    const int rows = 10;
    const int Espace = 0;

    int HauteurLigne = initialScreenHeight * 1 / 4;
    int TailleCarreWidth = (initialScreenWidth - (cols - 1) * Espace) / cols;
    int TailleCarreHeight = (initialScreenHeight - HauteurLigne - (rows - 1) * Espace) / rows;
    int TailleCarre = TailleCarreWidth < TailleCarreHeight ? TailleCarreWidth : TailleCarreHeight;

    int totalWidth = cols * TailleCarre + (cols - 1) * Espace;
    int availableSpace = initialScreenWidth - totalWidth;
    int availableSpacePerSide = availableSpace / 2;
    int additionalCols = availableSpacePerSide / (TailleCarre + Espace);

    
    Inventaire inventaire;
    InitInventaire(&inventaire);

    TexturesJeux textures;  
    InitTextures(&textures);

    Vector2 playerPosition = {(initialScreenWidth / 2) - 60, (initialScreenHeight / 4) - 120 };
    
    float echelle = 1.6*TailleCarre/textures.playerTextureIdle.height;


    int prevScreenWidth = initialScreenWidth;
    int prevScreenHeight = initialScreenHeight;

    int TailleBouton = 80;
    bool ParametreOuvert = false;
    bool InventaireOuvert = false ;
    int BoutonMenuWidth = 150;
    int BoutonMenuHeight = 40;
    bool fullscreen = false;

    bool isMovingRight = false;
    bool isMovingLeft = false;
    bool isAction = false;
    bool isMovingBas = false;
    bool isMovingHaut = false;

    int frameCounter = 0;

    #define NUM_MINERAIS 3
    Texture2D listeMinerais[NUM_MINERAIS] = {
    textures.Minerai_commun,
    textures.Minerai_rare,
    textures.Minerai_epique
    };

    TypeMinerai types[NUM_MINERAIS] = {COMMUN, RARE, EPIQUE};


    Bloc **Grille = NeedGrid(rows, cols, additionalCols, listeMinerais, textures.incassable,textures.evenement, types,NUM_MINERAIS);

    int Speed = 2;

    while (!WindowShouldClose()){

        int ScreenWidth = GetScreenWidth();
        int ScreenHeight = GetScreenHeight();
        
        if (ScreenWidth * 9 != ScreenHeight * 16) {
            if (ScreenWidth > ScreenHeight * 16 / 9) {
                ScreenWidth = ScreenHeight * 16 / 9;
            } else {
                ScreenHeight = ScreenWidth * 9 / 16;
            }
            SetWindowSize(ScreenWidth, ScreenHeight);
        }

        if (prevScreenHeight != ScreenHeight || prevScreenWidth != ScreenWidth){
            float widthScale = (float)ScreenWidth / prevScreenWidth;
            float heightScale = (float)ScreenHeight / prevScreenHeight;

            playerPosition.x *= widthScale;
            playerPosition.y *= heightScale;
            
            echelle *= heightScale;
            
            prevScreenHeight = ScreenHeight;
            prevScreenWidth = ScreenWidth;
        }

        HauteurLigne = ScreenHeight * 1 / 4;
        TailleCarreWidth = (ScreenWidth - (cols - 1) * Espace) / cols;
        TailleCarreHeight = (ScreenHeight - HauteurLigne - (rows - 1) * Espace) / rows;
        TailleCarre = TailleCarreWidth < TailleCarreHeight ? TailleCarreWidth : TailleCarreHeight;

        int startX= (ScreenWidth-((cols+2*additionalCols)*TailleCarre))/2;
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

        GetMouvements(Speed, ScreenWidth, ScreenHeight, &isAction, &isMovingRight, &isMovingLeft, &isMovingHaut, &isMovingBas, &playerPosition, textures, Grille, rows, cols, additionalCols, echelle);

        frameCounter++;

        BeginDrawing();
        
        ClearBackground(LIGHTGRAY);

        DrawTextureEx(textures.iconParametreTexture, PositionBoutonParametre,0.0,echelle, WHITE);

        if (TailleCarre < 1) TailleCarre = 1;
        
        
        if (ParametreOuvert) {
            DrawParametre(ScreenWidth, ScreenHeight, Pos1600, PosFull, PosRevenir, PosQuitter, BoutonMenuWidth, BoutonMenuHeight);
            ChekCollisionParametre(PosSouris, RetourButtonRect, ButtonRect1600, FullButtonRect, QuitterButtonRect, &ParametreOuvert, &fullscreen, &ScreenWidth, &ScreenHeight);
        } 
        else if (InventaireOuvert){
            DrawcompleteInventory(textures,&inventaire,&InventaireOuvert);
            CheckOuvertureInventaire(&InventaireOuvert);
            CheckOuvertureParametre(PosSouris, ParaRect, &ParametreOuvert);
        }else {
            CheckOuvertureInventaire(&InventaireOuvert);
            CheckOuvertureParametre(PosSouris, ParaRect, &ParametreOuvert);
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols + 2 * additionalCols; j++) {
                    if (VerifEtat(Grille[i][j])) {
                        continue;
                    }
                    int x = startX + j * (TailleCarre + Espace);
                    int y = startY + i * (TailleCarre + Espace);

                    DrawTextureEx(Grille[i][j].Texture, (Vector2){x, y}, 0.0f, (float)TailleCarre / Grille[i][j].Texture.width, WHITE);

                    Grille[i][j].HitBox = (Rectangle){x, y, TailleCarre, TailleCarre};

                    DetecterCollision((Rectangle) {playerPosition.x, playerPosition.y, (textures.playerTextureIdle.width) * echelle, (textures.playerTextureIdle.height) * echelle}, &Grille[i][j]);

                    SuprCliked(PosSouris, &Grille[i][j], &inventaire);
                }
            }
            DrawMouvements(isAction, isMovingRight, isMovingLeft, isMovingBas, isMovingHaut, frameCounter, playerPosition, textures, echelle);
            DrawInventaireQuick(&inventaire, HauteurLigne, TailleCarre);
        }
        EndDrawing();
    }
    UnloadTextures(&textures);
    FreeGrid(Grille, rows);
    CloseWindow();
    return 0;
}