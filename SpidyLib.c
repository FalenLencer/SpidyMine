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
                if (randValue>95){
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

void SuprCliked(Vector2 PosSouris , Bloc *cube ,Inventaire *inventaire){
    if (CheckCollisionPointRec(PosSouris, (*cube).HitBox)) {
        DrawRectangleLinesEx((*cube).HitBox, 2, RED);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && cube->type!=INCASSABLE) {
            (*cube).Etat = true;
            if ((*cube).type == COMMUN){
                inventaire->Mineraie_Niveau_01 = inventaire->Mineraie_Niveau_01+1;
            }
            if ((*cube).type == RARE){
                inventaire->Mineraie_Niveau_02 = inventaire->Mineraie_Niveau_02+1;
            }
            if ((*cube).type == EPIQUE){
                inventaire->Mineraie_Niveau_03 = inventaire->Mineraie_Niveau_03+1;
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

void FusionnerMinerais(Inventaire *inventaire) {
    if (inventaire->Mineraie_Niveau_01 >= 5 && inventaire->Mineraie_Niveau_02 >= 1) {
        inventaire->Mineraie_Niveau_01 -= 5;
        inventaire->Mineraie_Niveau_02 -= 1;
        inventaire->Mineraie_Niveau_03 += 1;
    }
}

void DrawcompleteInventory(TexturesJeux textures, Inventaire *inventaire, bool *InventaireOuvert) {

    int largeurEcran = GetScreenWidth();
    int hauteurEcran = GetScreenHeight();

    float echelleX = (float)largeurEcran / 1600.0f;
    float echelleY = (float)hauteurEcran / 900.0f;

    const int marge = 20 * echelleX;
    const int tailleIcône = 64 * echelleX;
    const int largeurFenetre = 600 * echelleX;
    const int hauteurFenetre = 300 * echelleY;

    int posX = (largeurEcran - largeurFenetre) / 2;
    int posY = (hauteurEcran - hauteurFenetre) / 2;

    DrawRectangle(posX, posY, largeurFenetre, hauteurFenetre, Fade(DARKGRAY, 0.8f));
    DrawRectangleLinesEx((Rectangle){posX, posY, largeurFenetre, hauteurFenetre}, 2, BLACK);

    DrawText("Inventaire", posX + marge, posY + marge, (int)(24 * echelleY), RAYWHITE);

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

    const int largeurFusion = 300 * echelleX;
    const int hauteurFusion = 200 * echelleY;
    const int posXFusion = posX + largeurFenetre + marge;
    const int posYFusion = posY;

    DrawRectangle(posXFusion, posYFusion, largeurFusion, hauteurFusion, Fade(DARKBLUE, 0.8f));
    DrawRectangleLinesEx((Rectangle){posXFusion, posYFusion, largeurFusion, hauteurFusion}, 2, BLACK);

    DrawText("Fusion de minerais", posXFusion + marge, posYFusion + marge, (int)(20 * echelleY), RAYWHITE);

    int texteFusionY = posYFusion + 60 * echelleY;
    DrawText("Prix Fusion EPIQUE:", posXFusion + marge, texteFusionY, (int)(18 * echelleY), WHITE);
    texteFusionY += 30 * echelleY;
    DrawText("5 x Commun + 1 x Rare", posXFusion + marge, texteFusionY, (int)(18 * echelleY), YELLOW);

    int largeurBouton = 120 * echelleX;
    int hauteurBouton = 40 * echelleY;
    int posXBouton = posXFusion + marge;
    int posYBouton = posYFusion + hauteurFusion - hauteurBouton - marge;

    bool peutFusionner = inventaire->Mineraie_Niveau_01 >= 5 && inventaire->Mineraie_Niveau_02 >= 1;

    if (peutFusionner) {
        DrawRectangle(posXBouton, posYBouton, largeurBouton, hauteurBouton, DARKGREEN);
        DrawText("Fusionner", posXBouton + 10 * echelleX, posYBouton + 10 * echelleY, (int)(20 * echelleY), WHITE);

        if (IsMouseOverRectangle(posXBouton, posYBouton, largeurBouton, hauteurBouton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            FusionnerMinerais(inventaire);
        }
    } else {
        DrawRectangle(posXBouton, posYBouton, largeurBouton, hauteurBouton, RED);
        DrawText("Impossible", posXBouton + 10 * echelleX, posYBouton + 10 * echelleY, (int)(20 * echelleY), WHITE);
    }
}
