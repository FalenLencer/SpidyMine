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

    Statistiques stats;
    InitStats(&stats);

    Vector2 playerPosition = {(initialScreenWidth / 2) - 60, (initialScreenHeight / 4) - 120 };
    
    float echelle = 1.6*TailleCarre/textures.playerTextureIdle.height;

    bool ParametreOuvert = false;
    bool InventaireOuvert = false ;
    bool IsEnding = false ;
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
    while (!WindowShouldClose()){
        int ScreenWidth = GetScreenWidth();
        int ScreenHeight = GetScreenHeight();
        /*
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
        }*/
        int TailleBouton = ProportionnelleHauteur(80, ScreenHeight);
        int BoutonMenuWidth = ProportionnelleLargeur(150, ScreenWidth);
        int BoutonMenuHeight = ProportionnelleHauteur(40, ScreenHeight);

        HauteurLigne = ProportionnelleHauteur(ScreenHeight / 4, ScreenHeight);
        TailleCarreWidth = ProportionnelleLargeur(67, ScreenWidth);
        TailleCarreHeight = ProportionnelleHauteur(67, ScreenHeight);
        TailleCarre = TailleCarreWidth < TailleCarreHeight ? TailleCarreWidth : TailleCarreHeight;

        int startX= (ScreenWidth-((cols+2*additionalCols)*TailleCarre))/2;
        int startY = HauteurLigne + Espace;

        Vector2 PositionBoutonParametre = {ProportionnelleLargeur(1480,ScreenWidth), ProportionnelleHauteur(10, ScreenHeight)};
        Vector2 Pos1600 = { ProportionnelleLargeur(725,ScreenWidth), ProportionnelleHauteur(350,ScreenHeight)};
        Vector2 PosFull = { ProportionnelleLargeur(725,ScreenWidth), ProportionnelleHauteur(400,ScreenHeight)};
        Vector2 PosRevenir = { ProportionnelleLargeur(725,ScreenWidth), ProportionnelleHauteur(450,ScreenHeight) };
        Vector2 PosQuitter = { ProportionnelleLargeur(725,ScreenWidth), ProportionnelleHauteur(500,ScreenHeight) };
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

        GetMouvements(stats.Vitesse, ScreenWidth, ScreenHeight, &isAction, &isMovingRight, &isMovingLeft, &isMovingHaut, &isMovingBas, &playerPosition, textures, Grille, rows, cols, additionalCols, echelle);

        frameCounter++;

        BeginDrawing();
        
        ClearBackground(LIGHTGRAY);

        DrawTextureEx(textures.iconParametreTexture, PositionBoutonParametre,0.0,echelle, WHITE);

        if (TailleCarre < 1) TailleCarre = 1;

        Rectangle Personnage={playerPosition.x, playerPosition.y, (textures.playerTextureIdle.width) * echelle, (textures.playerTextureIdle.height) * echelle};
        
        if (ParametreOuvert) {
            DrawParametre(ScreenWidth, ScreenHeight, Pos1600, PosFull, PosRevenir, PosQuitter, BoutonMenuWidth, BoutonMenuHeight);
            ChekCollisionParametre(PosSouris, RetourButtonRect, ButtonRect1600, FullButtonRect, QuitterButtonRect, &ParametreOuvert, &fullscreen, &ScreenWidth, &ScreenHeight);
        } 
        else if (InventaireOuvert){
            DrawCompleteInventory(rows,cols , additionalCols , NUM_MINERAIS ,&IsEnding, textures,  &inventaire,&stats,&Grille);
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

                    DetecterCollision(Personnage , &Grille[i][j]);

                    SuprCliked(PosSouris, &Grille[i][j], &inventaire, stats);
                }
            }
            if (IsEnding){
                DrawTextureEx(textures.PortailFin,(Vector2){((cols+additionalCols)*TailleCarre)+startX,0},0.0,ProportionnelleHauteur(0.235,ScreenHeight),WHITE);
                Rectangle Recportail={((cols+additionalCols)*TailleCarre)+startX,0 ,textures.PortailFin.width*ProportionnelleHauteur(0.235,ScreenHeight),textures.PortailFin.height*ProportionnelleHauteur(0.235,ScreenHeight)};
                DrawRectangleLinesEx(Recportail, 2 ,BLUE);
                if (CheckCollisionRecs(Personnage,Recportail)){
                    break;
                }
            }
            DrawMouvements(isAction, isMovingRight, isMovingLeft, isMovingBas, isMovingHaut, frameCounter, playerPosition, textures, echelle);
            DrawInventaireQuick(&inventaire, ScreenHeight, ScreenWidth);
        }
        EndDrawing();
    }
    UnloadTextures(&textures);
    FreeGrid(Grille, rows);
    CloseWindow();
    return 0;
}
