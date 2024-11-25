#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SpidyLib.h"
#include "CalculLib.h"

Texture2D LoadTextureIfExists(const char *imagePath) {
    if (strlen(imagePath) > 0 && FileExists(imagePath)) {
        return LoadTexture(imagePath);
    }
    return (Texture2D){0};
}

int VerifEtat(Bloc B) {
    return (B.Etat == true);
}

Bloc **NeedGrid(int rows, int cols, int additionalCols, Texture2D Minerais[],  Texture incassable, Texture evenement, TypeMinerai Types[],int NUM_MINERAIS) {
  
    Bloc **Grille = malloc(sizeof(Bloc*) * rows);

    for (int p = 0; p < rows; p++) {
        Grille[p] = malloc(sizeof(Bloc) * (cols + 2 * additionalCols));
    }

    int *Stat01 = GetPourcentageDecroissant(NUM_MINERAIS, 95);
    int *Stat02 = GetPourcentagePick(NUM_MINERAIS,95);
    int *Stat03 = GetPourcentageCroissant(NUM_MINERAIS, 95);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < (cols + 2 * additionalCols); j++) {
            Grille[i][j].Etat = false;
            Grille[i][j].PosX = j;
            Grille[i][j].PosY = i;

            if (j < additionalCols || j >= (cols + additionalCols)) {
                Grille[i][j].Texture = incassable;
                Grille[i][j].type = INCASSABLE;
                if (i > 0 && j != additionalCols - 1 && j != (cols + additionalCols)) {
                    Grille[i][j].Etat = true;
                }
            } else {
                int *stats;
                if (i < (cols + 2 * additionalCols) / 3) {
                    stats = Stat01;
                } else if (i < 2 * (cols + 2 * additionalCols) / 3) {
                    stats = Stat02;
                } else {
                    stats = Stat03;
                }

                int randValue = rand() % 100;
                int cumulative = 0;

                for (int m = 0; m < NUM_MINERAIS; m++) {
                    cumulative += stats[m];
                    if (randValue < cumulative) {
                        Grille[i][j].Texture = Minerais[m];
                        Grille[i][j].type = Types[m];
                        break;
                    }
                }
                if (randValue>=95){
                    Grille[i][j].Texture = evenement;
                    Grille[i][j].type = EVENEMENT;
                }
            }

            Grille[i][j].HitBox = (Rectangle){0, 0, 0, 0};
        }
    }

    free(Stat01);
    free(Stat02);
    free(Stat03);

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

void InitInventaire(Inventaire *inventaire) {
    (*inventaire).Mineraie_Niveau_01=0;
    (*inventaire).Mineraie_Niveau_02=0;
    (*inventaire).Mineraie_Niveau_03=0;
}

void InitStats(Statistiques *stats) {
    (*stats).Fortune=0;
    (*stats).Vitesse=2;
    (*stats).Niveau=0;
}

void DrawInventaireQuick(Inventaire *inventaire, int HauteurLigne , int TailleCarre) {
    DrawText(TextFormat("Minerai commun : %d",inventaire->Mineraie_Niveau_01), 0,HauteurLigne+40+TailleCarre ,20,GREEN );
    DrawText(TextFormat("Minerai rare : %d",inventaire->Mineraie_Niveau_02), 0,HauteurLigne+60+TailleCarre ,20,BLUE );
    DrawText(TextFormat("Minerai épique : %d",inventaire->Mineraie_Niveau_03), 0,HauteurLigne+80+TailleCarre ,20,PURPLE );
}
void InitTextures(TexturesJeux *textures) {
    (*textures).Minerai_commun = LoadTextureIfExists("Texture_Blocs/Nut.png");
    (*textures).Minerai_rare = LoadTextureIfExists("Texture_Blocs/Minerai1.png");
    (*textures).Minerai_epique = LoadTextureIfExists("Texture_Blocs/Mineraie_epique.PNG");
    (*textures).evenement = LoadTextureIfExists("Texture_Blocs/Evenement.png");
    (*textures).incassable = LoadTextureIfExists("Texture_Blocs/unbreakable.png");
    (*textures).iconParametreTexture = LoadTextureIfExists("parametre.png");
    (*textures).backgroundTexture = LoadTextureIfExists("background.png");
    (*textures).playerTextureIdle = LoadTextureIfExists("Mouvement_Perso/neutre.png");
    (*textures).playerTextureMove = LoadTextureIfExists("Mouvement_Perso/bouge 1.png");
    (*textures).playerTextureMoveD = LoadTextureIfExists("Mouvement_Perso/bouge 2.png");
    (*textures).playerTextureAction = LoadTextureIfExists("Mouvement_Perso/mine 1.png");
    (*textures).playerTextureAction2 = LoadTextureIfExists("Mouvement_Perso/mine 2.png");
    (*textures).playerTextureHaut = LoadTextureIfExists("Mouvement_Perso/perso de dos.png");
    (*textures).playerTextureMove2 = LoadTextureIfExists("Mouvement_Perso/perso bouge gauche 1.png");
    (*textures).playerTextureMove2g = LoadTextureIfExists("Mouvement_Perso/perso bouge gauche 2.png");
}

void UnloadTextures(TexturesJeux *textures) {
    if ((*textures).Minerai_commun.id > 0) UnloadTexture((*textures).Minerai_commun);
    if ((*textures).Minerai_rare.id > 0) UnloadTexture((*textures).Minerai_rare);
    if ((*textures).Minerai_epique.id > 0) UnloadTexture((*textures).Minerai_epique);
    if ((*textures).evenement.id > 0) UnloadTexture((*textures).evenement); 
    if ((*textures).incassable.id > 0) UnloadTexture((*textures).incassable); 
    if ((*textures).iconParametreTexture.id > 0) UnloadTexture((*textures).iconParametreTexture);
    if ((*textures).backgroundTexture.id > 0) UnloadTexture((*textures).backgroundTexture);
    if ((*textures).playerTextureIdle.id > 0) UnloadTexture((*textures).playerTextureIdle);
    if ((*textures).playerTextureMove.id > 0) UnloadTexture((*textures).playerTextureMove);
    if ((*textures).playerTextureMoveD.id > 0) UnloadTexture((*textures).playerTextureMoveD);
    if ((*textures).playerTextureAction.id > 0) UnloadTexture((*textures).playerTextureAction);
    if ((*textures).playerTextureMove2.id > 0) UnloadTexture((*textures).playerTextureMove2);
    if ((*textures).playerTextureHaut.id > 0) UnloadTexture((*textures).playerTextureHaut);
    if ((*textures).playerTextureMove2.id > 0) UnloadTexture((*textures).playerTextureMove2);
    if ((*textures).playerTextureMove2g.id > 0) UnloadTexture((*textures).playerTextureMove2g);
}

bool IsCollidingWithBloc(Rectangle personnage, Bloc **Grille, int rows, int cols, int additionalCols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols+2*additionalCols; j++) {
            Bloc bloc = Grille[i][j];
            if (!bloc.Etat && CheckCollisionRecs(personnage, bloc.HitBox)) {
                return true;
            }
        }
    }
    return false;
}

void GetMouvements(int Speed, int ScreenWidth, int ScreenHeight, bool *isAction, bool *isMovingRight, bool *isMovingLeft, bool *isMovingHaut, bool *isMovingBas, Vector2 *playerPosition, TexturesJeux textures, Bloc **Grille, int rows, int cols ,int additionalCols , float echelle) {
    Rectangle personnage = (Rectangle) {(*playerPosition).x,(*playerPosition).y,(textures.playerTextureIdle.width)*echelle,(textures.playerTextureIdle.height)*echelle};
    
    if (IsKeyDown(KEY_SPACE)) {
        *isAction = true;
    } else {
        Vector2 newPosition = *playerPosition;

        if (IsKeyDown(KEY_RIGHT)) {
            newPosition.x += Speed;
            personnage.x = newPosition.x;
            if (newPosition.x <= ScreenWidth - textures.playerTextureIdle.width && !IsCollidingWithBloc(personnage, Grille, rows, cols,additionalCols)) {
                (*playerPosition).x = newPosition.x;
                *isMovingRight = true;
            }
        }
        if (IsKeyDown(KEY_LEFT)) {
            newPosition.x -= Speed;
            personnage.x = newPosition.x;
            if (newPosition.x >= 0 && !IsCollidingWithBloc(personnage, Grille, rows, cols,additionalCols)) {
                (*playerPosition).x = newPosition.x;
                *isMovingLeft = true;
            }
        }
        if (IsKeyDown(KEY_UP)) {
            newPosition.y -= Speed;
            personnage.y = newPosition.y;
            if (newPosition.y >= 0 && !IsCollidingWithBloc(personnage, Grille, rows, cols,additionalCols)) {
                (*playerPosition).y = newPosition.y;
                *isMovingHaut = true;
            }
        }
        if (IsKeyDown(KEY_DOWN)) {
            newPosition.y += Speed;
            personnage.y = newPosition.y;
            if (newPosition.y + textures.playerTextureIdle.height <= ScreenHeight && !IsCollidingWithBloc(personnage, Grille, rows, cols,additionalCols)) {
                (*playerPosition).y = newPosition.y;
                *isMovingBas = true;
            }
        }
    }
}


void DrawMouvements(bool isAction,bool isMovingRight,bool isMovingLeft ,bool isMovingBas,bool isMovingHaut, int frameCounter,Vector2 playerPosition ,TexturesJeux textures, float echelle){
     if (isAction) {
        if ((frameCounter / 15) % 2 == 0) {
            Vector2 newpos=(playerPosition);
            newpos.y= newpos.y-(5*echelle);
            DrawTextureEx(textures.playerTextureAction, newpos, 0.0, echelle, WHITE);
        }else{
             Vector2 newpos=(playerPosition);
            newpos.y= newpos.y-(10*echelle);
            DrawTextureEx(textures.playerTextureAction2, newpos, 0.0, echelle, WHITE);
        }
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

void SuprCliked(Vector2 PosSouris , Bloc *cube ,Inventaire *inventaire,Statistiques stats){
    srand(time(NULL));
    int oprotunite_commun = rand() % 5;
    int oprotunite_rare = rand() % 10;
    int oprotunite_epique = rand() % 15;
    if (CheckCollisionPointRec(PosSouris, (*cube).HitBox)) {
        DrawRectangleLinesEx((*cube).HitBox, 2, RED);
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && cube->type!=INCASSABLE) {
            (*cube).Etat = true;
            if ((*cube).type == COMMUN){
                if (oprotunite_commun<stats.Fortune){
                    inventaire->Mineraie_Niveau_01 = inventaire->Mineraie_Niveau_01+2;
                }
                else{
                    inventaire->Mineraie_Niveau_01 = inventaire->Mineraie_Niveau_01+1;
                }
                
            }
            if ((*cube).type == RARE){
                if (oprotunite_rare<stats.Fortune){
                    inventaire->Mineraie_Niveau_02 = inventaire->Mineraie_Niveau_02+2;
                }
                else{
                    inventaire->Mineraie_Niveau_02 = inventaire->Mineraie_Niveau_02+1;
                }
            }
            if ((*cube).type == EPIQUE){
                if (oprotunite_epique<stats.Fortune){
                    inventaire->Mineraie_Niveau_03 = inventaire->Mineraie_Niveau_03+2;
                }
                else{
                    inventaire->Mineraie_Niveau_03 = inventaire->Mineraie_Niveau_03+1;
                }
            }
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

void CheckOuvertureInventaire(bool *InventaireOuvert){
    if (IsKeyPressed(KEY_TAB)){
        (*InventaireOuvert)= !(*InventaireOuvert) ;
    }
}

bool IsMouseOverRectangle(int x, int y, int width, int height) {
    Vector2 mousePosition = GetMousePosition();
    return mousePosition.x >= x && mousePosition.x <= x + width && 
           mousePosition.y >= y && mousePosition.y <= y + height;
}
void FusionnerMineraisRare(Inventaire *inventaire) {
    if (inventaire->Mineraie_Niveau_01 >= 5 ) {
        inventaire->Mineraie_Niveau_01 -= 5;
        inventaire->Mineraie_Niveau_02 += 1;
    }
}

void FusionnerMineraisEpique(Inventaire *inventaire) {
    if (inventaire->Mineraie_Niveau_01 >= 15 && inventaire->Mineraie_Niveau_02 >= 5) {
        inventaire->Mineraie_Niveau_01 -= 5;
        inventaire->Mineraie_Niveau_02 -= 1;
        inventaire->Mineraie_Niveau_03 += 1;
    }
}
void FusionnerMineraisLegendaire(Inventaire *inventaire) {
    if (inventaire->Mineraie_Niveau_01 >= 100 && inventaire->Mineraie_Niveau_02 >= 50 && inventaire->Mineraie_Niveau_03>=25) {
        inventaire->Mineraie_Niveau_01 -= 100;
        inventaire->Mineraie_Niveau_02 -= 50;
        inventaire->Mineraie_Niveau_03 -= 25;
        inventaire->Mineraie_Niveau_04 += 1;
    }
}
void AmeliorerFortune(Inventaire *inventaire, Statistiques *stats,int prix) {
    if (inventaire->Mineraie_Niveau_02 >= prix && stats->Fortune<=5) {
            inventaire->Mineraie_Niveau_02 -= prix;
            (*stats).Fortune+=1 ;        
    }
}

void AmeliorerVitesseDeplacement(Inventaire *inventaire, Statistiques *stats, int prix) {
    if (inventaire->Mineraie_Niveau_02 >= prix) {
        inventaire->Mineraie_Niveau_02 -= prix;
        stats->Vitesse += 1;
    }
}

void HandleFusionRare(int posXFusion,int posYFusion,int echelleX , int echelleY , int marge ,int posXBoutonFusion , int posYBoutonFusion, int largeurBouton , int hauteurBouton , Inventaire *inventaire){
    int texteFusionY = posYBoutonFusion - 90 * echelleY;
    DrawText("Prix Fusion RARE:", posXFusion + marge, texteFusionY, (int)(20 * echelleY), WHITE);
    texteFusionY += 30 * echelleY;
    DrawText("5 x Commun", posXFusion + marge, texteFusionY, (int)(18 * echelleY), YELLOW);

    bool peutFusionner = inventaire->Mineraie_Niveau_01 >= 5;

    if (peutFusionner) {
        DrawRectangle(posXBoutonFusion, posYBoutonFusion, largeurBouton, hauteurBouton, DARKGREEN);
        DrawText("Fusionner", posXBoutonFusion + 10 * echelleX, posYBoutonFusion + 10 * echelleY, (int)(20 * echelleY), WHITE);

        if (IsMouseOverRectangle(posXBoutonFusion, posYBoutonFusion, largeurBouton, hauteurBouton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            FusionnerMineraisRare(inventaire);
        }
    } else {
        DrawRectangle(posXBoutonFusion, posYBoutonFusion, largeurBouton, hauteurBouton, RED);
        DrawText("Impossible", posXBoutonFusion + 10 * echelleX, posYBoutonFusion + 10 * echelleY, (int)(20 * echelleY), WHITE);
    }
}

void HandleFusionEpique(int posXFusion,int posYFusion,int echelleX , int echelleY , int marge ,int posXBoutonFusion , int posYBoutonFusion, int largeurBouton , int hauteurBouton , Inventaire *inventaire){

    int texteFusionY =posYBoutonFusion - 90 * echelleY;
    DrawText("Prix Fusion EPIQUE:", posXFusion + marge, texteFusionY, (int)(20 * echelleY), WHITE);
    texteFusionY += 30 * echelleY;
    DrawText("15xCommun + 5xRare", posXFusion + marge, texteFusionY, (int)(18 * echelleY), YELLOW);

    bool peutFusionner = inventaire->Mineraie_Niveau_01 >= 15 && inventaire->Mineraie_Niveau_02 >= 5;

    if (peutFusionner) {
        DrawRectangle(posXBoutonFusion, posYBoutonFusion, largeurBouton, hauteurBouton, DARKGREEN);
        DrawText("Fusionner", posXBoutonFusion + 10 * echelleX, posYBoutonFusion + 10 * echelleY, (int)(20 * echelleY), WHITE);

        if (IsMouseOverRectangle(posXBoutonFusion, posYBoutonFusion, largeurBouton, hauteurBouton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            FusionnerMineraisEpique(inventaire);
        }
    } else {
        DrawRectangle(posXBoutonFusion, posYBoutonFusion, largeurBouton, hauteurBouton, RED);
        DrawText("Impossible", posXBoutonFusion + 10 * echelleX, posYBoutonFusion + 10 * echelleY, (int)(20 * echelleY), WHITE);
    }
}
void HandleFusionLegendaire(int posXFusion,int posYFusion,int echelleX , int echelleY , int marge ,int posXBoutonFusion , int posYBoutonFusion, int largeurBouton , int hauteurBouton , Inventaire *inventaire){
    
    int texteFusionY = posYBoutonFusion - 90 * echelleY;
    DrawText("Prix Fusion LEGENDAIRE:", posXFusion + marge, texteFusionY, (int)(20 * echelleY), WHITE);
    texteFusionY += 30 * echelleY;
    DrawText("100xCommun + 50xRare \n+ 25xEpique", posXFusion + marge, texteFusionY, (int)(18 * echelleY), YELLOW);

    bool peutFusionner = inventaire->Mineraie_Niveau_01 >= 100 && inventaire->Mineraie_Niveau_02 >= 50 && inventaire->Mineraie_Niveau_03>=25;

    if (peutFusionner) {
        DrawRectangle(posXBoutonFusion, posYBoutonFusion, largeurBouton, hauteurBouton, DARKGREEN);
        DrawText("Fusionner", posXBoutonFusion + 10 * echelleX, posYBoutonFusion + 10 * echelleY, (int)(20 * echelleY), WHITE);

        if (IsMouseOverRectangle(posXBoutonFusion, posYBoutonFusion, largeurBouton, hauteurBouton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            FusionnerMineraisLegendaire(inventaire);
        }
    } else {
        DrawRectangle(posXBoutonFusion, posYBoutonFusion, largeurBouton, hauteurBouton, RED);
        DrawText("Impossible", posXBoutonFusion + 10 * echelleX, posYBoutonFusion + 10 * echelleY, (int)(20 * echelleY), WHITE);
    }

}

void HandleFusion(int echelleX,int echelleY , int posX,int posY , int marge ,int largeurBouton,int hauteurBouton, int largeurFenetre,int hauteurEcran, Inventaire *inventaire){
    const int largeurFusion = 300 * echelleX;
    const int hauteurFusion = (hauteurEcran-posY-marge) * echelleY;
    const int posXFusion = posX + largeurFenetre + marge;
    const int posYFusion = posY;

    DrawRectangle(posXFusion, posYFusion, largeurFusion, hauteurFusion, Fade((Color){160,140,255,0}, 0.8f));
    DrawRectangleLinesEx((Rectangle){posXFusion, posYFusion, largeurFusion, hauteurFusion}, 2, BLACK);

    DrawText("Fusion de minerais", posXFusion + marge, posYFusion + marge, (int)(20 * echelleY), RAYWHITE);

    int posXBoutonFusion = posXFusion + marge;
    int posYBoutonFusionRare =posYFusion + hauteurFusion - hauteurBouton - marge -380*echelleY;;
    int posYBoutonFusionEpique =posYFusion + hauteurFusion - hauteurBouton - marge-190*echelleY;; 
    int posYBoutonFusionLegendaire = posYFusion + hauteurFusion - hauteurBouton - marge;

    HandleFusionRare(posXFusion,posYFusion,echelleX , echelleY , marge ,posXBoutonFusion , posYBoutonFusionRare, largeurBouton , hauteurBouton , inventaire);
    HandleFusionEpique(posXFusion, posYFusion,echelleX , echelleY , marge ,posXBoutonFusion , posYBoutonFusionEpique, largeurBouton , hauteurBouton , inventaire);
    HandleFusionLegendaire(posXFusion,posYFusion,echelleX , echelleY , marge ,posXBoutonFusion ,  posYBoutonFusionLegendaire, largeurBouton , hauteurBouton , inventaire);

}

void HandleDecraft(int posX , int posY , int hauteurFenetre , int largeurFenetre , int marge, int echelleX, int echelleY, int largeurBouton, int hauteurBouton){
    
}

void HandleInventory(int posX,int posY , int largeurFenetre, int hauteurFenetre ,int marge , int echelleY,int echelleX, TexturesJeux textures , Inventaire *inventaire){
    
    DrawRectangle(posX, posY, largeurFenetre, hauteurFenetre, Fade(DARKGRAY, 0.8f));
    DrawRectangleLinesEx((Rectangle){posX, posY, largeurFenetre, hauteurFenetre}, 2, BLACK);

    DrawText("Inventaire", posX + marge, posY + marge, (int)(24 * echelleY), RAYWHITE);
    
    const int tailleIcône = 64 * echelleX;
    int decalageTexteY = posY + 2 * marge + 24 * echelleY;
    int decalageIcôneX = posX + marge; 
    int decalageTexteX = decalageIcôneX + tailleIcône + marge;

    float facteurEchelle = (float)tailleIcône / textures.Minerai_commun.width;
    DrawTextureEx(textures.Minerai_commun, (Vector2){decalageIcôneX, decalageTexteY}, 0.0f, facteurEchelle, WHITE);
    DrawText(TextFormat("Minerai commun : %d", inventaire->Mineraie_Niveau_01), decalageTexteX, decalageTexteY + tailleIcône / 4, (int)(20 * echelleY), GREEN);

    decalageTexteY += tailleIcône + marge;
    DrawTextureEx(textures.Minerai_rare, (Vector2){decalageIcôneX, decalageTexteY}, 0.0f, facteurEchelle, WHITE);
    DrawText(TextFormat("Minerai rare : %d", inventaire->Mineraie_Niveau_02), decalageTexteX, decalageTexteY + tailleIcône / 4, (int)(20 * echelleY), BLUE);

    decalageTexteY += tailleIcône + marge;
    facteurEchelle = (float)tailleIcône / textures.Minerai_epique.width;
    DrawTextureEx(textures.Minerai_epique, (Vector2){decalageIcôneX, decalageTexteY}, 0.0f, facteurEchelle, WHITE);
    DrawText(TextFormat("Minerai épique : %d", inventaire->Mineraie_Niveau_03), decalageTexteX, decalageTexteY + tailleIcône / 4, (int)(20 * echelleY), PURPLE);

}

void HandleFortune(int posXAmelioration, int posYAmelioration ,int marge, int largeurAmelioration , int hauteurAmelioration , int echelleX , int echelleY ,int largeurBouton,int hauteurBouton, Inventaire *inventaire, Statistiques *stats){
    DrawRectangle(posXAmelioration, posYAmelioration, largeurAmelioration, hauteurAmelioration, Fade(DARKGRAY, 0.8f));
    DrawRectangleLinesEx((Rectangle){posXAmelioration, posYAmelioration, largeurAmelioration, hauteurAmelioration}, 2, BLACK);

    DrawText("Fortune :", posXAmelioration + marge, posYAmelioration + marge, (int)(20 * echelleY), RAYWHITE);
    DrawText("Augmente les chances des drops de minerais", posXAmelioration + marge, posYAmelioration + 50 * echelleY, (int)(18 * echelleY), YELLOW);

    int prixAmelioration = 10;
    DrawText(TextFormat("Prix : %d minerais rares", prixAmelioration), posXAmelioration + marge, posYAmelioration + 90 * echelleY, (int)(18 * echelleY), WHITE);

    int posXBoutonAmelioration = posXAmelioration + marge;
    int posYBoutonAmelioration = posYAmelioration + hauteurAmelioration - hauteurBouton - marge;

    bool peutAmeliorer = inventaire->Mineraie_Niveau_02 >= prixAmelioration;

    if (peutAmeliorer) {
        DrawRectangle(posXBoutonAmelioration, posYBoutonAmelioration, largeurBouton, hauteurBouton, DARKGREEN);
        DrawText("Améliorer", posXBoutonAmelioration + 10 * echelleX, posYBoutonAmelioration + 10 * echelleY, (int)(20 * echelleY), WHITE);

        if (IsMouseOverRectangle(posXBoutonAmelioration, posYBoutonAmelioration, largeurBouton, hauteurBouton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            AmeliorerFortune(inventaire,stats,prixAmelioration);
        }
    } else {
        DrawRectangle(posXBoutonAmelioration, posYBoutonAmelioration, largeurBouton, hauteurBouton, RED);
        DrawText("Impossible", posXBoutonAmelioration + 10 * echelleX, posYBoutonAmelioration + 10 * echelleY, (int)(20 * echelleY), WHITE);
    }
    
}

void HandleVitesseDeplacement(int posXAmelioration, int posYAmelioration ,int marge, int largeurAmelioration , int hauteurAmelioration , int echelleX , int echelleY ,int largeurBouton,int hauteurBouton, Inventaire *inventaire, Statistiques *stats){
    const int posYAmeliorationVitesse = posYAmelioration + hauteurAmelioration + 20 * echelleY;
    DrawRectangle(posXAmelioration, posYAmeliorationVitesse, largeurAmelioration, hauteurAmelioration, Fade(DARKGRAY, 0.8f));
    DrawRectangleLinesEx((Rectangle){posXAmelioration, posYAmeliorationVitesse, largeurAmelioration, hauteurAmelioration}, 2, BLACK);

    DrawText("Vitesse :", posXAmelioration + marge, posYAmeliorationVitesse + marge, (int)(20 * echelleY), RAYWHITE);
    DrawText("Augmente la vitesse de déplacement", posXAmelioration + marge, posYAmeliorationVitesse + 50 * echelleY, (int)(18 * echelleY), YELLOW);

    int prixAmeliorationVitesse = 15;
    DrawText(TextFormat("Prix : %d minerais rares", prixAmeliorationVitesse), posXAmelioration + marge, posYAmeliorationVitesse + 90 * echelleY, (int)(18 * echelleY), WHITE);

    int posXBoutonVitesse = posXAmelioration + marge;
    int posYBoutonVitesse = posYAmeliorationVitesse + hauteurAmelioration - hauteurBouton - marge;

    bool peutAmeliorerVitesse = inventaire->Mineraie_Niveau_02 >= prixAmeliorationVitesse;

    if (peutAmeliorerVitesse) {
        DrawRectangle(posXBoutonVitesse, posYBoutonVitesse, largeurBouton, hauteurBouton, DARKGREEN);
        DrawText("Améliorer", posXBoutonVitesse + 10 * echelleX, posYBoutonVitesse + 10 * echelleY, (int)(20 * echelleY), WHITE);

        if (IsMouseOverRectangle(posXBoutonVitesse, posYBoutonVitesse, largeurBouton, hauteurBouton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            AmeliorerVitesseDeplacement(inventaire, stats, prixAmeliorationVitesse);
        }
    } else {
        DrawRectangle(posXBoutonVitesse, posYBoutonVitesse, largeurBouton, hauteurBouton, RED);
        DrawText("Impossible", posXBoutonVitesse + 10 * echelleX, posYBoutonVitesse + 10 * echelleY, (int)(20 * echelleY), WHITE);
    }
}

void HandleUpgrades(int posX , int posY , int marge , int hauteurEcran , int echelleX , int echelleY ,int largeurBouton , int hauteurBouton, Inventaire *inventaire , Statistiques *stats){
    const int largeurRectangleGauche = posX - 2 * marge;
    const int hauteurRectangleGauche = hauteurEcran - 2 * marge; 

    DrawRectangle(marge, marge, largeurRectangleGauche, hauteurRectangleGauche, Fade(DARKBLUE, 0.8f));
    DrawRectangleLinesEx((Rectangle){marge, marge, largeurRectangleGauche, hauteurRectangleGauche}, 2, BLACK);
    DrawText("Améliorations :", (largeurRectangleGauche / 2) - MeasureText("Améliorations :", (int)(20 * echelleY)) / 2, marge + marge, (int)(20 * echelleY), RAYWHITE);

    const int largeurAmelioration = largeurRectangleGauche - 2 * marge;
    const int hauteurAmelioration = 200 * echelleY;
    const int posXAmelioration = marge + marge;
    const int posYAmelioration = marge + 60 * echelleY;

    HandleFortune( posXAmelioration,  posYAmelioration , marge,  largeurAmelioration ,  hauteurAmelioration ,  echelleX ,  echelleY ,largeurBouton,hauteurBouton , inventaire,  stats);
    HandleVitesseDeplacement( posXAmelioration,  posYAmelioration , marge,  largeurAmelioration ,  hauteurAmelioration ,  echelleX ,  echelleY ,largeurBouton,hauteurBouton, inventaire, stats);
}


void DrawCompleteInventory(TexturesJeux textures, Inventaire *inventaire, Statistiques *stats) {
    int largeurEcran = GetScreenWidth();
    int hauteurEcran = GetScreenHeight();

    float echelleX = (float)largeurEcran / 1600.0f;
    float echelleY = (float)hauteurEcran / 900.0f;

    const int marge = 20 * echelleX;
    const int largeurFenetre = 600 * echelleX;
    const int hauteurFenetre = 300 * echelleY;

    int posX = (largeurEcran - largeurFenetre) / 2;
    int posY = (hauteurEcran - hauteurFenetre) / 2;

    int largeurBouton = 120 * echelleX;
    int hauteurBouton = 40 * echelleY;

    HandleUpgrades( posX ,  posY ,  marge ,  hauteurEcran ,  echelleX ,  echelleY ,largeurBouton,hauteurBouton  ,inventaire , stats);
    HandleInventory( posX, posY ,  largeurFenetre,  hauteurFenetre , marge ,  echelleY, echelleX,  textures ,  inventaire);
    
    HandleFusion( echelleX, echelleY ,  posX, posY ,  marge ,largeurBouton,hauteurBouton,  largeurFenetre,hauteurEcran, inventaire);
    HandleDecraft( posX ,  posY ,  hauteurFenetre ,  largeurFenetre ,  marge , echelleX , echelleY, largeurBouton, hauteurBouton);

}
