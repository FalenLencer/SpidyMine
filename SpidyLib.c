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

float ProportionnelleLargeur(float valeur, int ScreenWidth) {
    return (valeur / 1600.0f) * ScreenWidth;
}

float ProportionnelleHauteur(float valeur, int ScreenHeight) {
    return (valeur / 900.0f) * ScreenHeight;
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
    int *Stat02 = GetPourcentagePick(NUM_MINERAIS, 95);
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
            Grille[i][j].PeutMiner=false;
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
    DrawRectangle(ProportionnelleLargeur(600,ScreenWidth), ProportionnelleHauteur(300,ScreenHeight), ProportionnelleLargeur(400,ScreenWidth), ProportionnelleHauteur(300,ScreenHeight), GREEN);
    DrawText("Paramètre", ProportionnelleLargeur(750,ScreenWidth), ProportionnelleHauteur(320,ScreenHeight), 20, BLACK);
    DrawRectangle(Pos1600.x, Pos1600.y, BoutonMenuWidth, BoutonMenuHeight, DARKGRAY);
    DrawText("1600x900", Pos1600.x + ProportionnelleLargeur(15,ScreenWidth), Pos1600.y + ProportionnelleHauteur(10,ScreenHeight), 20, RAYWHITE);
    DrawRectangle(PosFull.x, PosFull.y, BoutonMenuWidth, BoutonMenuHeight, DARKGRAY);
    DrawText("Plein Écran", PosFull.x + ProportionnelleLargeur(15,ScreenWidth), PosFull.y + ProportionnelleHauteur(10,ScreenHeight), 20, RAYWHITE);
    DrawRectangle(PosRevenir.x, PosRevenir.y, BoutonMenuWidth, BoutonMenuHeight, DARKGRAY);
    DrawText("Revenir", PosRevenir.x + ProportionnelleLargeur(15,ScreenWidth), PosRevenir.y + ProportionnelleHauteur(10,ScreenHeight), 20, RAYWHITE);
    DrawRectangle(PosQuitter.x, PosQuitter.y, BoutonMenuWidth, BoutonMenuHeight, RED);
    DrawText("Quitter", PosQuitter.x + ProportionnelleLargeur(15,ScreenWidth), PosQuitter.y + ProportionnelleHauteur(10,ScreenHeight), 20, RAYWHITE);
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
    (*inventaire).Mineraie_Niveau_04=0;
}

void InitStats(Statistiques *stats) {
    (*stats).Fortune=0;
    (*stats).Vitesse=2;
    (*stats).Niveau=0;
}

void DrawInventaireQuick(Inventaire *inventaire, int hauteurEcran , int largeurEcran) {
    int tailleText= ProportionnelleHauteur(20,hauteurEcran);
    DrawText(TextFormat("Minerai commun : %d",inventaire->Mineraie_Niveau_01), 0,ProportionnelleHauteur(325,hauteurEcran) ,tailleText,GREEN );
    DrawText(TextFormat("Minerai rare : %d",inventaire->Mineraie_Niveau_02), 0,ProportionnelleHauteur(345,hauteurEcran) ,tailleText,BLUE );
    DrawText(TextFormat("Minerai épique : %d",inventaire->Mineraie_Niveau_03), 0,ProportionnelleHauteur(365,hauteurEcran) ,tailleText,PURPLE );
    DrawText(TextFormat("Minerai Legendaire : %d",inventaire->Mineraie_Niveau_04), 0,ProportionnelleHauteur(385,hauteurEcran) ,tailleText,YELLOW );
}
void InitTextures(TexturesJeux *textures) {
    (*textures).Minerai_commun = LoadTextureIfExists("Texture_Blocs/Nut.png");
    (*textures).Minerai_rare = LoadTextureIfExists("Texture_Blocs/Minerai1.png");
    (*textures).Minerai_epique = LoadTextureIfExists("Texture_Blocs/Mineraie_epique.PNG");
    (*textures).evenement = LoadTextureIfExists("Texture_Blocs/Evenement.png");
    (*textures).incassable = LoadTextureIfExists("Texture_Blocs/unbreakable.png");
    (*textures).iconParametreTexture = LoadTextureIfExists("parametre.png");
    (*textures).backgroundTexture = LoadTextureIfExists("background.png");
    (*textures).playerTextureIdle = LoadTextureIfExists("Personnage/Vue_de_face.png");
    (*textures).playerTextureMoveDroite1 = LoadTextureIfExists("Personnage/marche_droite(1).png");
    (*textures).playerTextureMoveDroite2 = LoadTextureIfExists("Personnage/marche_droite(2).png");
    (*textures).playerTextureAction = LoadTextureIfExists("Personnage/Animation_minage/mine_à_droite_(1).png");
    (*textures).playerTextureAction2 = LoadTextureIfExists("Personnage/Animation_minage/mine_à_droite_(2).png");
    (*textures).playerTextureHaut = LoadTextureIfExists("Personnage/monté(pied_droit).png");
    (*textures).playerTextureHaut2 = LoadTextureIfExists("Personnage/monté(pied_gauche).png");
    (*textures).playerTextureMoveGauche1 = LoadTextureIfExists("Personnage/marche_gauche(1).png");
    (*textures).playerTextureMoveGauche2 = LoadTextureIfExists("Personnage/marche_gauche(2).png");
    (*textures).playerTextureBas = LoadTextureIfExists("Personnage/descente(pied_droit).png");
    (*textures).playerTextureBas2 = LoadTextureIfExists("Personnage/descente(pied_gauche).png");
    (*textures).PortailNewmine = LoadTextureIfExists("Texture_Blocs/portailMine.png");
    (*textures).PortailFin = LoadTextureIfExists("Texture_Blocs/portailfin.png");
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
    if ((*textures).playerTextureMoveDroite1.id > 0) UnloadTexture((*textures).playerTextureMoveDroite1);
    if ((*textures).playerTextureMoveDroite2.id > 0) UnloadTexture((*textures).playerTextureMoveDroite2);
    if ((*textures).playerTextureAction.id > 0) UnloadTexture((*textures).playerTextureAction);
    if ((*textures).playerTextureAction2.id > 0) UnloadTexture((*textures).playerTextureAction2);
    if ((*textures).playerTextureHaut.id > 0) UnloadTexture((*textures).playerTextureHaut);
    if ((*textures).playerTextureHaut2.id > 0) UnloadTexture((*textures).playerTextureHaut2);
    if ((*textures).playerTextureMoveGauche1.id > 0) UnloadTexture((*textures).playerTextureMoveGauche1);
    if ((*textures).playerTextureMoveGauche2.id > 0) UnloadTexture((*textures).playerTextureMoveGauche2);
    if ((*textures).playerTextureBas.id > 0) UnloadTexture((*textures).playerTextureBas);
    if ((*textures).playerTextureBas2.id > 0) UnloadTexture((*textures).playerTextureBas2);
    if ((*textures).PortailNewmine.id > 0) UnloadTexture((*textures).PortailNewmine);
    if ((*textures).PortailFin.id > 0) UnloadTexture((*textures).PortailFin);
}

bool IsCollidingWithBloc(Rectangle personnage, Bloc ***Grille, int rows, int cols, int additionalCols) {
    bool colliding=false;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols+2*additionalCols; j++) {
            if (!(*Grille)[i][j].Etat && CheckCollisionRecs(personnage, (*Grille)[i][j].HitBox)) {
                (*Grille)[i][j].PeutMiner=true;
                colliding=true;
            }
            else{
                (*Grille)[i][j].PeutMiner=false;
            }
        }
    }
    return colliding;
}

void GetMouvements(int Speed, int ScreenWidth, int ScreenHeight, bool *isAction, bool *isMovingRight, bool *isMovingLeft, bool *isMovingHaut, bool *isMovingBas, Vector2 *playerPosition, TexturesJeux textures, Bloc ***Grille, int rows, int cols ,int additionalCols , float echelle) {
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
            DrawTextureEx(textures.playerTextureAction, playerPosition, 0.0, echelle, WHITE);
        }else{
            DrawTextureEx(textures.playerTextureAction2, playerPosition, 0.0, echelle, WHITE);
        }
        return;
    }
    if (isMovingRight) {
        if ((frameCounter / 15) % 2 == 0) {
            DrawTextureEx(textures.playerTextureMoveDroite1, playerPosition, 0.0, echelle, WHITE);
        } else {
            DrawTextureEx(textures.playerTextureMoveDroite2, playerPosition, 0.0, echelle, WHITE);
        }
    }
    else if (isMovingLeft) {
        if ((frameCounter / 15) % 2 == 0) {
            DrawTextureEx(textures.playerTextureMoveGauche1, playerPosition, 0.0, echelle, WHITE);
        } else {
            DrawTextureEx(textures.playerTextureMoveGauche2, playerPosition, 0.0, echelle, WHITE);
        } 
    }
    else if (isMovingBas) {
        if ((frameCounter / 15) % 2 == 0) {
            DrawTextureEx(textures.playerTextureBas, playerPosition, 0.0, echelle, WHITE);
        } else {
            DrawTextureEx(textures.playerTextureBas2, playerPosition, 0.0, echelle, WHITE);
        } 
    }
    else if (isMovingHaut) {
        if ((frameCounter / 15) % 2 == 0) {
            DrawTextureEx(textures.playerTextureHaut, playerPosition, 0.0, echelle, WHITE);
        } else {
            DrawTextureEx(textures.playerTextureHaut2, playerPosition, 0.0, echelle, WHITE);
        } 
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
    if (/*CheckCollisionPointRec(PosSouris, (*cube).HitBox)&&*/ cube->PeutMiner && cube->type!=INCASSABLE ) {
        DrawRectangleLinesEx((*cube).HitBox, 2, RED);
        if (/*IsMouseButtonPressed(MOUSE_BUTTON_LEFT)*/IsKeyDown(KEY_SPACE)  ) {
            (*cube).Etat = true;
            if ((*cube).type == BLOC_EVENEMENT) {
                declencherPiege();
            }
            if ((*cube).type == COMMUN)
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
        else if (IsKeyPressed(KEY_T) && cube->type==INCASSABLE){
            inventaire->Mineraie_Niveau_04+=1;
        }
    }


void CheckOuvertureParametre( Vector2 PosSouris, Rectangle ParaRect ,bool *ParametreOuvert){
    if (CheckMouseCollisionCliked(PosSouris, ParaRect)) {
        *ParametreOuvert = true;  
    }
}

void DetecterCollision(Rectangle Personage, Bloc *Cube){
    DrawRectangleLines(Personage.x,Personage.y,Personage.width,Personage.height,ORANGE);
    if(CheckCollisionRecs((*Cube).HitBox,Personage)| Cube->PeutMiner){
        DrawRectangleLinesEx((*Cube).HitBox, 2, BLUE);
    }
}
void declencherPiege() {
    printf("Un piège s'est déclenché\n");
    sante -= 20; 
    if (sante < 0) {
        sante = 0
    }
    printf("santé restante : %d\n", sante);
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
void DecraftRare(Inventaire * inventaire){
    if (inventaire->Mineraie_Niveau_02 >= 1){
        inventaire->Mineraie_Niveau_02-=1;
        inventaire->Mineraie_Niveau_01+=5;
    }
}
void FusionnerMineraisEpique(Inventaire *inventaire) {
    if (inventaire->Mineraie_Niveau_01 >= 15 && inventaire->Mineraie_Niveau_02 >= 5) {
        inventaire->Mineraie_Niveau_01 -= 5;
        inventaire->Mineraie_Niveau_02 -= 1;
        inventaire->Mineraie_Niveau_03 += 1;
    }
}
void DecraftEpique(Inventaire * inventaire){
    if (inventaire->Mineraie_Niveau_03 >= 1){
        inventaire->Mineraie_Niveau_03-=1;
        inventaire->Mineraie_Niveau_01 += 5;
        inventaire->Mineraie_Niveau_02 += 1;
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
void DecraftLegendaire(Inventaire * inventaire){
    if (inventaire->Mineraie_Niveau_04 >= 1){
        inventaire->Mineraie_Niveau_04-=1;
        inventaire->Mineraie_Niveau_01 += 100;
        inventaire->Mineraie_Niveau_02 += 50;
        inventaire->Mineraie_Niveau_03 += 25;
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

void HandleFusionRare(int posXFusion,int posYFusion,int margeX , int margeY ,int posXBoutonFusion , int posYBoutonFusion,int hauteurEcran,int largeurEcran, int largeurBouton , int hauteurBouton , Inventaire *inventaire){
    int texteFusionY = posYBoutonFusion - ProportionnelleHauteur(90,hauteurEcran);
    int TailleText=ProportionnelleHauteur(20,hauteurEcran);
    DrawText("Prix Fusion RARE:", posXFusion + margeX, texteFusionY, TailleText, WHITE);
    texteFusionY += ProportionnelleHauteur(30,hauteurEcran) ;
    DrawText("5 x Commun", posXFusion + margeX, texteFusionY, ProportionnelleHauteur(18,hauteurEcran), YELLOW);

    bool peutFusionner = inventaire->Mineraie_Niveau_01 >= 5;
    int ecartX=ProportionnelleLargeur(10,largeurEcran);
    int ecartY=ProportionnelleHauteur(10,hauteurEcran);
    if (peutFusionner) {
        DrawRectangle(posXBoutonFusion, posYBoutonFusion, largeurBouton, hauteurBouton, DARKGREEN);
        DrawText("Fusionner", posXBoutonFusion +  ecartX, posYBoutonFusion + ecartY, TailleText, WHITE);

        if (IsMouseOverRectangle(posXBoutonFusion, posYBoutonFusion, largeurBouton, hauteurBouton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            FusionnerMineraisRare(inventaire);
        }
    } else {
        DrawRectangle(posXBoutonFusion, posYBoutonFusion, largeurBouton, hauteurBouton, RED);
        DrawText("Impossible", posXBoutonFusion + ecartX, posYBoutonFusion+ecartY, TailleText, WHITE);
    }
}

void HandleFusionEpique(int posXFusion,int posYFusion,int margeX , int margeY ,int posXBoutonFusion , int posYBoutonFusion,int hauteurEcran,int largeurEcran, int largeurBouton , int hauteurBouton , Inventaire *inventaire){

    int texteFusionY =posYBoutonFusion - ProportionnelleHauteur(90,hauteurEcran);
    int TailleText=ProportionnelleHauteur(20,hauteurEcran);
    DrawText("Prix Fusion EPIQUE:",posXFusion + margeX, texteFusionY, TailleText, WHITE);
    texteFusionY +=ProportionnelleHauteur(30,hauteurEcran) ;
    DrawText("15xCommun + 5xRare", posXFusion + margeX, texteFusionY, ProportionnelleHauteur(18,hauteurEcran), YELLOW);

    bool peutFusionner = inventaire->Mineraie_Niveau_01 >= 15 && inventaire->Mineraie_Niveau_02 >= 5;
    int ecartX=ProportionnelleLargeur(10,largeurEcran);
    int ecartY=ProportionnelleHauteur(10,hauteurEcran);
    if (peutFusionner) {
        DrawRectangle(posXBoutonFusion, posYBoutonFusion, largeurBouton, hauteurBouton, DARKGREEN);
        DrawText("Fusionner", posXBoutonFusion +ecartX, posYBoutonFusion + ecartY, TailleText, WHITE);

        if (IsMouseOverRectangle(posXBoutonFusion, posYBoutonFusion, largeurBouton, hauteurBouton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            FusionnerMineraisEpique(inventaire);
        }
    } else {
        DrawRectangle(posXBoutonFusion, posYBoutonFusion, largeurBouton, hauteurBouton, RED);
        DrawText("Impossible", posXBoutonFusion + ecartX, posYBoutonFusion + ecartY, TailleText, WHITE);
    }
}
void HandleFusionLegendaire(int posXFusion,int posYFusion,int margeX , int margeY ,int posXBoutonFusion , int posYBoutonFusion,int hauteurEcran,int largeurEcran, int largeurBouton , int hauteurBouton , Inventaire *inventaire){
    
    int texteFusionY =posYBoutonFusion - ProportionnelleHauteur(90,hauteurEcran);
    int TailleText=ProportionnelleHauteur(20,hauteurEcran);
    DrawText("Prix Fusion LEGENDAIRE:", posXFusion + margeX, texteFusionY, TailleText, WHITE);
    texteFusionY += ProportionnelleHauteur(30,hauteurEcran);
    DrawText("100xCommun + 50xRare \n+ 25xEpique", posXFusion + margeX, texteFusionY, ProportionnelleHauteur(18,hauteurEcran), YELLOW);

    bool peutFusionner = inventaire->Mineraie_Niveau_01 >= 100 && inventaire->Mineraie_Niveau_02 >= 50 && inventaire->Mineraie_Niveau_03>=25;
    int ecartX=ProportionnelleLargeur(10,largeurEcran);
    int ecartY=ProportionnelleHauteur(10,hauteurEcran);
    if (peutFusionner) {
        DrawRectangle(posXBoutonFusion, posYBoutonFusion, largeurBouton, hauteurBouton, DARKGREEN);
        DrawText("Fusionner", posXBoutonFusion +ecartX, posYBoutonFusion + ecartY, TailleText, WHITE);

        if (IsMouseOverRectangle(posXBoutonFusion, posYBoutonFusion, largeurBouton, hauteurBouton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            FusionnerMineraisLegendaire(inventaire);
        }
    } else {
        DrawRectangle(posXBoutonFusion, posYBoutonFusion, largeurBouton, hauteurBouton, RED);
        DrawText("Impossible", posXBoutonFusion + ecartX, posYBoutonFusion +ecartY, TailleText, WHITE);
    }

}

void HandleFusion(int posX,int posY , int margeX, int margeY ,int largeurBouton,int hauteurBouton, int hauteurFenetre,int largeurFenetre ,int hauteurEcran,int largeurEcran, Inventaire *inventaire){
    const int largeurFusion = ProportionnelleLargeur(300,largeurEcran);
    const int hauteurFusion = ProportionnelleHauteur(580,hauteurEcran);
    const int posXFusion = posX + largeurFenetre + margeX;
    const int posYFusion = posY;
    int TailleText=ProportionnelleHauteur(20,hauteurEcran);
    
    DrawRectangle(posXFusion, posYFusion, largeurFusion, hauteurFusion, Fade((Color){160,140,255,0}, 0.8f));
    DrawRectangleLinesEx((Rectangle){posXFusion, posYFusion, largeurFusion, hauteurFusion}, 2, BLACK);

    DrawText("Fusion de minerais", posXFusion + margeX, posYFusion + margeY, TailleText, RAYWHITE);

    int posXBoutonFusion = posXFusion + margeX;
    int posYBoutonFusionRare =posYFusion + hauteurFusion - hauteurBouton - margeY -ProportionnelleHauteur(380,hauteurEcran);
    int posYBoutonFusionEpique =posYFusion + hauteurFusion - hauteurBouton - margeY-ProportionnelleHauteur(190,hauteurEcran);
    int posYBoutonFusionLegendaire = posYFusion + hauteurFusion - hauteurBouton - margeY;

    HandleFusionRare(posXFusion,posYFusion,margeX , margeY ,posXBoutonFusion , posYBoutonFusionRare, hauteurEcran, largeurEcran, largeurBouton , hauteurBouton , inventaire);
    HandleFusionEpique(posXFusion,posYFusion,margeX , margeY ,posXBoutonFusion , posYBoutonFusionEpique, hauteurEcran, largeurEcran, largeurBouton , hauteurBouton , inventaire);
    HandleFusionLegendaire(posXFusion,posYFusion,margeX , margeY ,posXBoutonFusion , posYBoutonFusionLegendaire, hauteurEcran, largeurEcran, largeurBouton , hauteurBouton , inventaire);

}


void HandleDecraftRare(int posX, int posY, int largeurBouton, int hauteurBouton, int ecartX,int ecartY,int TailleText, Inventaire *inventaire) {
    
    bool peutdecraftRare=inventaire->Mineraie_Niveau_02 >= 1;
    if (peutdecraftRare) {
        DrawRectangle(posX, posY, largeurBouton, hauteurBouton, DARKBLUE);
        DrawText("Rare", posX + ecartX, posY + (hauteurBouton / 3), TailleText, WHITE);

        if (IsMouseOverRectangle(posX, posY, largeurBouton, hauteurBouton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            DecraftRare(inventaire);
        }
    } else {
        DrawRectangle(posX, posY, largeurBouton, hauteurBouton, RED);
        DrawText("Impossible", posX + ecartX, posY +ecartY, TailleText, WHITE);
    }
}
void HandleDecraftEpique(int posX, int posY, int largeurBouton, int hauteurBouton, int ecartX,int ecartY,int TailleText, Inventaire *inventaire) {
    bool peutdecraftEpique=inventaire->Mineraie_Niveau_03 >= 1;
    if (peutdecraftEpique) {
        DrawRectangle(posX, posY, largeurBouton, hauteurBouton, PURPLE);
        DrawText("Epique", posX + ecartX, posY + (hauteurBouton / 3), 20, WHITE);

        if (IsMouseOverRectangle(posX, posY, largeurBouton, hauteurBouton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            DecraftEpique(inventaire);
        }
    } else {
        DrawRectangle(posX, posY, largeurBouton, hauteurBouton, RED);
        DrawText("Impossible", posX + ecartX, posY +ecartY, TailleText, WHITE);
    }
}

void HandleDecraftLegendaire(int posX, int posY, int largeurBouton, int hauteurBouton, int ecartX,int ecartY,int TailleText, Inventaire *inventaire) {
    
    bool peutdecraftLegendaire=inventaire->Mineraie_Niveau_04 >= 1;
    if (peutdecraftLegendaire) {
        DrawRectangle(posX, posY, largeurBouton, hauteurBouton, GOLD);
        DrawText("Legendaire", posX + ecartX, posY + (hauteurBouton / 3), 20, WHITE);
        if (IsMouseOverRectangle(posX, posY, largeurBouton, hauteurBouton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            DecraftLegendaire(inventaire);
        }
    } else {
        DrawRectangle(posX, posY, largeurBouton, hauteurBouton, RED);
        DrawText("Impossible", posX + ecartX, posY +ecartY, TailleText, WHITE);
    }

}

void HandleDecraft(int posX, int posY, int margeX, int margeY,int largeurBouton , int hauteurBouton, int hauteurEcran, int largeurEcran, int hauteurFenetre, int largeurFenetre, TexturesJeux textures, Inventaire *inventaire) {
    int TailleText = ProportionnelleHauteur(20, hauteurEcran);
    int posXdecraft = posX;
    int posYdecraft = posY + hauteurFenetre + margeY;
    int largeurDecraft = largeurFenetre;
    int hauteurDecraft = ProportionnelleHauteur(260, hauteurEcran);

    DrawRectangle(posXdecraft, posYdecraft, largeurDecraft, hauteurDecraft, Fade((Color){160, 140, 255, 0}, 0.8f));
    DrawRectangleLinesEx((Rectangle){posXdecraft, posYdecraft, largeurDecraft, hauteurDecraft}, 2, BLACK);
    DrawText("Déconstruction de minerais", posXdecraft + margeX, posYdecraft + margeY, TailleText, RAYWHITE);
    DrawText("Permet de deconstruire son minerais pour recuperer \nses éléments qui le forme.", posXdecraft + margeX, posYdecraft + 4*margeY, TailleText, RAYWHITE);

    int ecartBoutons = ProportionnelleLargeur(100, largeurEcran);

    int posXBoutonDecraftRare = posXdecraft + margeX;
    int posXBoutonDecraftEpique = posXBoutonDecraftRare + largeurBouton + ecartBoutons;
    int posXBoutonDecraftLegendaire = posXBoutonDecraftEpique + largeurBouton + ecartBoutons;
    int posYBoutonDecraft = posYdecraft + hauteurDecraft - hauteurBouton - margeY;

    int ecartX=ProportionnelleLargeur(10,largeurEcran);
    int ecartY=ProportionnelleHauteur(10,hauteurEcran);

    HandleDecraftRare(posXBoutonDecraftRare, posYBoutonDecraft, largeurBouton, hauteurBouton, ecartX,ecartY,TailleText, inventaire);
    HandleDecraftEpique(posXBoutonDecraftEpique, posYBoutonDecraft, largeurBouton, hauteurBouton, ecartX,ecartY,TailleText, inventaire);
    HandleDecraftLegendaire(posXBoutonDecraftLegendaire, posYBoutonDecraft, largeurBouton, hauteurBouton, ecartX,ecartY, TailleText,inventaire);
}


void HandleInventory(int posX, int posY , int margeX,int margeY ,int hauteurEcran , int largeurEcran,int hauteurFenetre,int largeurFenetre , TexturesJeux textures , Inventaire *inventaire){
    int TailleText=ProportionnelleHauteur(20,hauteurEcran);
    DrawRectangle(posX, posY, largeurFenetre, hauteurFenetre, Fade(DARKGRAY, 0.8f));
    DrawRectangleLinesEx((Rectangle){posX, posY, largeurFenetre, hauteurFenetre}, 2, BLACK);

    DrawText("Inventaire", posX + margeX, posY + margeY, (int)(ProportionnelleHauteur(24,hauteurEcran)), RAYWHITE);
    
    const int tailleIcône = ProportionnelleLargeur(64,largeurEcran);
    int decalageTexteY = posY + 2 * margeY + ProportionnelleHauteur(24,hauteurEcran);
    int decalageIcôneX = posX + margeX; 
    int decalageTexteX = decalageIcôneX + tailleIcône + margeX;

    float facteurEchelle = (float)tailleIcône / textures.Minerai_commun.width;
    DrawTextureEx(textures.Minerai_commun, (Vector2){decalageIcôneX, decalageTexteY}, 0.0f, facteurEchelle, WHITE);
    DrawText(TextFormat("Minerai commun : %d", inventaire->Mineraie_Niveau_01), decalageTexteX, decalageTexteY + tailleIcône / 4, TailleText, GREEN);

    decalageTexteY += tailleIcône + margeY;
    DrawTextureEx(textures.Minerai_rare, (Vector2){decalageIcôneX, decalageTexteY}, 0.0f, facteurEchelle, WHITE);
    DrawText(TextFormat("Minerai rare : %d", inventaire->Mineraie_Niveau_02), decalageTexteX, decalageTexteY + tailleIcône / 4,TailleText, BLUE);

    decalageTexteY += tailleIcône + margeY;
    facteurEchelle = (float)tailleIcône / textures.Minerai_epique.width;
    DrawTextureEx(textures.Minerai_epique, (Vector2){decalageIcôneX, decalageTexteY}, 0.0f, facteurEchelle, WHITE);
    DrawText(TextFormat("Minerai épique : %d", inventaire->Mineraie_Niveau_03), decalageTexteX, decalageTexteY + tailleIcône / 4, TailleText, PURPLE);

}

void HandleFortune( int hauteurEcran , int largeurEcran,int margeX,int margeY ,int hauteurBouton,int largeurBouton, int Tailletext , Inventaire *inventaire, Statistiques *stats){
    int dimRectX=ProportionnelleLargeur(420,largeurEcran);
    int dimRectY=ProportionnelleHauteur(200,hauteurEcran);
    int posRectX=ProportionnelleLargeur(40,largeurEcran);
    int posRectY=ProportionnelleHauteur(80,hauteurEcran);
    DrawRectangle(posRectX, posRectY, dimRectX, dimRectY, Fade(DARKGRAY, 0.8f));
    DrawRectangleLinesEx((Rectangle){posRectX, posRectY, dimRectX, dimRectY}, 2, BLACK);

    DrawText("Fortune :", posRectX+ margeX, posRectY + margeY, Tailletext, RAYWHITE);
    DrawText("Augmente les chances des drops de minerais", posRectX + margeX, posRectY+ProportionnelleHauteur(70,hauteurEcran), (int)(ProportionnelleLargeur(18,largeurEcran)), YELLOW);

    int prixAmelioration = 10;
    DrawText(TextFormat("Prix : %d minerais rares", prixAmelioration), posRectX + margeX, posRectY+ProportionnelleHauteur(110,hauteurEcran), (int)(ProportionnelleLargeur(18,largeurEcran)), WHITE);

    int posXBoutonAmelioration = posRectX + margeX;
    int posYBoutonAmelioration = posRectY + dimRectY - hauteurBouton - margeY;

    bool peutAmeliorer = inventaire->Mineraie_Niveau_02 >= prixAmelioration;

    if (peutAmeliorer) {
        DrawRectangle(posXBoutonAmelioration, posYBoutonAmelioration, largeurBouton, hauteurBouton, DARKGREEN);
        DrawText("Améliorer", posXBoutonAmelioration +ProportionnelleLargeur(10,largeurEcran), posYBoutonAmelioration + ProportionnelleHauteur(10,hauteurEcran), Tailletext, WHITE);

        if (IsMouseOverRectangle(posXBoutonAmelioration, posYBoutonAmelioration, largeurBouton, hauteurBouton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            AmeliorerFortune(inventaire,stats,prixAmelioration);
        }
    } else {
        DrawRectangle(posXBoutonAmelioration, posYBoutonAmelioration, largeurBouton, hauteurBouton, RED);
        DrawText("Impossible", posXBoutonAmelioration +ProportionnelleLargeur(10,largeurEcran), posYBoutonAmelioration + ProportionnelleHauteur(10,hauteurEcran), Tailletext, WHITE);
    }
    
}

void HandleVitesseDeplacement( int hauteurEcran , int largeurEcran,int margeX,int margeY ,int hauteurBouton,int largeurBouton, int Tailletext , Inventaire *inventaire, Statistiques *stats){
    int dimRectX=ProportionnelleLargeur(420,largeurEcran);
    int dimRectY=ProportionnelleHauteur(200,hauteurEcran);
    int posRectX=ProportionnelleLargeur(40,largeurEcran);
    int posRectY=ProportionnelleHauteur(280,hauteurEcran)+margeY;
    DrawRectangle(posRectX, posRectY, dimRectX, dimRectY, Fade(DARKGRAY, 0.8f));
    DrawRectangleLinesEx((Rectangle){posRectX, posRectY, dimRectX, dimRectY}, 2, BLACK);

    DrawText("Vitesse :", posRectX+ margeX, posRectY + margeY, Tailletext, RAYWHITE);
    DrawText("Augmente la vitesse de déplacement", posRectX + margeX, posRectY+ProportionnelleHauteur(70,hauteurEcran), (int)(ProportionnelleLargeur(18,largeurEcran)), YELLOW);

    int prixAmeliorationVitesse = 15;
    DrawText(TextFormat("Prix : %d minerais rares", prixAmeliorationVitesse), posRectX + margeX, posRectY+ProportionnelleHauteur(110,hauteurEcran), (int)(ProportionnelleLargeur(18,largeurEcran)), WHITE);

    int posXBoutonAmelioration = posRectX + margeX;
    int posYBoutonAmelioration = posRectY + dimRectY - hauteurBouton - margeY;

    bool peutAmeliorerVitesse = inventaire->Mineraie_Niveau_02 >= prixAmeliorationVitesse;

    if (peutAmeliorerVitesse) {
        DrawRectangle(posXBoutonAmelioration, posYBoutonAmelioration, largeurBouton, hauteurBouton, DARKGREEN);
        DrawText("Améliorer", posXBoutonAmelioration +ProportionnelleLargeur(10,largeurEcran), posYBoutonAmelioration + ProportionnelleHauteur(10,hauteurEcran), Tailletext, WHITE);

        if (IsMouseOverRectangle(posXBoutonAmelioration, posYBoutonAmelioration, largeurBouton, hauteurBouton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            AmeliorerVitesseDeplacement(inventaire, stats, prixAmeliorationVitesse);
        }
    } else {
        DrawRectangle(posXBoutonAmelioration, posYBoutonAmelioration, largeurBouton, hauteurBouton, RED);
        DrawText("Impossible", posXBoutonAmelioration +ProportionnelleLargeur(10,largeurEcran), posYBoutonAmelioration + ProportionnelleHauteur(10,hauteurEcran), Tailletext, WHITE);
    }
}

void Changerportail(TexturesJeux textures, Inventaire *inventaire, Bloc ***Grille, int rows, int cols, int additionalCols, int NUM_MINERAIS, int prix) {
    if (inventaire->Mineraie_Niveau_02 >= prix) {
        inventaire->Mineraie_Niveau_02 -= prix;

        Texture2D *listeMinerais = malloc(NUM_MINERAIS * sizeof(Texture2D));
        TypeMinerai *types = malloc(NUM_MINERAIS * sizeof(TypeMinerai));

        for (int i = 0; i < NUM_MINERAIS; ++i) {
            if (i == 0) {
                listeMinerais[i] = textures.Minerai_commun;
                types[i] = COMMUN;
            } else if (i == 1) {
                listeMinerais[i] = textures.Minerai_rare;
                types[i] = RARE;
            } else if (i == 2) {
                listeMinerais[i] = textures.Minerai_epique;
                types[i] = EPIQUE;
            }
        }

        FreeGrid(*Grille, rows);
        *Grille = NeedGrid(rows, cols, additionalCols, listeMinerais, textures.incassable, textures.evenement, types, NUM_MINERAIS);

        free(listeMinerais);
        free(types);
    }
}

void HandleNewPortal( int hauteurEcran , int largeurEcran,int margeX,int margeY ,int hauteurBouton,int largeurBouton, int Tailletext , int rows, int cols ,  int additionalCols ,  int NUM_MINERAIS,Inventaire *inventaire, Statistiques *stats,Bloc ***Grille, TexturesJeux textures){
    int dimRectX=ProportionnelleLargeur(420,largeurEcran);
    int dimRectY=ProportionnelleHauteur(200,hauteurEcran);
    int posRectX=ProportionnelleLargeur(40,largeurEcran);
    int posRectY=ProportionnelleHauteur(500,hauteurEcran)+margeY;
    DrawRectangle(posRectX, posRectY, dimRectX, dimRectY, Fade(DARKGRAY, 0.8f));
    DrawRectangleLinesEx((Rectangle){posRectX, posRectY, dimRectX, dimRectY}, 2, BLACK);

    DrawText("Nouveau portail :", posRectX+ margeX, posRectY + margeY, Tailletext, RAYWHITE);
    DrawText("Rénitialise la grille actuelle", posRectX + margeX, posRectY+ProportionnelleHauteur(70,hauteurEcran), (int)(ProportionnelleLargeur(18,largeurEcran)), YELLOW);

    int prixPortail = 10;
    DrawText(TextFormat("Prix : %d minerais rares", prixPortail), posRectX + margeX, posRectY+ProportionnelleHauteur(110,hauteurEcran), (int)(ProportionnelleLargeur(18,largeurEcran)), WHITE);

    int posXBoutonAmelioration = posRectX + margeX;
    int posYBoutonAmelioration = posRectY + dimRectY - hauteurBouton - margeY;

    bool peutacheterportail = inventaire->Mineraie_Niveau_02 >= prixPortail;

    if (peutacheterportail) {
        DrawRectangle(posXBoutonAmelioration, posYBoutonAmelioration, largeurBouton, hauteurBouton, DARKGREEN);
        DrawText("Acheter", posXBoutonAmelioration +ProportionnelleLargeur(10,largeurEcran), posYBoutonAmelioration + ProportionnelleHauteur(10,hauteurEcran), Tailletext, WHITE);

        if (IsMouseOverRectangle(posXBoutonAmelioration, posYBoutonAmelioration, largeurBouton, hauteurBouton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Changerportail(textures, inventaire, Grille, rows, cols, additionalCols, NUM_MINERAIS, prixPortail);

        }
    } else {
        DrawRectangle(posXBoutonAmelioration, posYBoutonAmelioration, largeurBouton, hauteurBouton, RED);
        DrawText("Impossible", posXBoutonAmelioration +ProportionnelleLargeur(10,largeurEcran), posYBoutonAmelioration + ProportionnelleHauteur(10,hauteurEcran), Tailletext, WHITE);
    }
}

void HandleLastPortal( int hauteurEcran , int largeurEcran,int margeX,int margeY ,int hauteurBouton,int largeurBouton, int Tailletext , Inventaire *inventaire, Statistiques *stats, bool *IsEnding){
    int dimRectX=ProportionnelleLargeur(420,largeurEcran);
    int dimRectY=ProportionnelleHauteur(120,hauteurEcran);
    int posRectX=ProportionnelleLargeur(40,largeurEcran);
    int posRectY=ProportionnelleHauteur(720,hauteurEcran)+margeY;
    DrawRectangle(posRectX, posRectY, dimRectX, dimRectY, Fade(DARKGRAY, 0.8f));
    DrawRectangleLinesEx((Rectangle){posRectX, posRectY, dimRectX, dimRectY}, 2, BLACK);

    DrawText("????", posRectX+ margeX, posRectY + margeY, Tailletext, RAYWHITE);
    
    int prixPortail = 1;
    int posXBoutonAmelioration = posRectX + margeX;
    int posYBoutonAmelioration = posRectY + dimRectY - hauteurBouton - margeY;

    bool Peutacheter = inventaire->Mineraie_Niveau_04 >= prixPortail;

    if (Peutacheter) {
        DrawRectangle(posXBoutonAmelioration, posYBoutonAmelioration, largeurBouton, hauteurBouton, DARKGREEN);
        DrawText("Acheter", posXBoutonAmelioration +ProportionnelleLargeur(10,largeurEcran), posYBoutonAmelioration + ProportionnelleHauteur(10,hauteurEcran), Tailletext, WHITE);

        if (IsMouseOverRectangle(posXBoutonAmelioration, posYBoutonAmelioration, largeurBouton, hauteurBouton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            *IsEnding=true;
        }
    } else {
        DrawRectangle(posXBoutonAmelioration, posYBoutonAmelioration, largeurBouton, hauteurBouton, RED);
        DrawText("Impossible", posXBoutonAmelioration +ProportionnelleLargeur(10,largeurEcran), posYBoutonAmelioration + ProportionnelleHauteur(10,hauteurEcran), Tailletext, WHITE);
    }
}
    


void HandleUpgrades(int posX , int posY , int margeX ,int margeY, int hauteurEcran ,int largeurEcran, int largeurBouton , int hauteurBouton,int rows,int cols ,int additionalCols , int NUM_MINERAIS, bool *IsEnding ,Inventaire *inventaire , Statistiques *stats,Bloc ***Grille, TexturesJeux textures){
    int dimX=ProportionnelleLargeur(460,largeurEcran);
    int dimY=ProportionnelleHauteur(860,hauteurEcran);
    int tailleText= ProportionnelleHauteur(20,hauteurEcran);
    DrawRectangle(margeX, margeY,dimX , dimY, Fade(DARKBLUE, 0.8f));
    DrawRectangleLinesEx((Rectangle){margeX, margeY, dimX, dimY}, 2, BLACK);
    DrawText("Boutique :", (int) (margeX+(dimX/2)-(MeasureText("Boutique :",tailleText)/2)), margeY + margeY, tailleText, RAYWHITE);

    HandleFortune(  hauteurEcran ,  largeurEcran, margeX, margeY , hauteurBouton, largeurBouton,  tailleText ,  inventaire,  stats);
    HandleVitesseDeplacement( hauteurEcran ,  largeurEcran, margeX, margeY , hauteurBouton, largeurBouton,  tailleText ,  inventaire,  stats);
    HandleNewPortal(  hauteurEcran ,  largeurEcran, margeX, margeY , hauteurBouton, largeurBouton, tailleText , rows, cols , additionalCols , NUM_MINERAIS,inventaire, stats,Grille,  textures);
    HandleLastPortal(hauteurEcran ,  largeurEcran, margeX, margeY , hauteurBouton, largeurBouton,  tailleText ,  inventaire,  stats , IsEnding);
}


void DrawCompleteInventory(int rows,int cols , int additionalCols , int NUM_MINERAIS ,bool *IsEnding,TexturesJeux textures, Inventaire *inventaire, Statistiques *stats,Bloc ***Grille) {
    int largeurEcran = GetScreenWidth();
    int hauteurEcran = GetScreenHeight();


    const int margeX = ProportionnelleHauteur(20,hauteurEcran);
    const int margeY = ProportionnelleLargeur(20,largeurEcran);

    const int largeurFenetre = ProportionnelleLargeur(600,largeurEcran);
    const int hauteurFenetre = ProportionnelleHauteur(300,hauteurEcran);

    int posX = (largeurEcran - largeurFenetre) / 2;
    int posY = (hauteurEcran - hauteurFenetre) / 2;

    int largeurBouton = ProportionnelleLargeur(120,largeurEcran);
    int hauteurBouton = ProportionnelleHauteur(40,hauteurEcran);

    HandleUpgrades(posX , posY , margeX ,margeY, hauteurEcran ,largeurEcran, largeurBouton , hauteurBouton,rows, cols , additionalCols , NUM_MINERAIS,IsEnding, inventaire ,stats,Grille, textures);
    HandleInventory( posX,  posY ,  margeX, margeY , hauteurEcran ,  largeurEcran, hauteurFenetre, largeurFenetre , textures , inventaire);
    HandleFusion(posX, posY ,  margeX,  margeY , largeurBouton, hauteurBouton,  hauteurFenetre, largeurFenetre , hauteurEcran, largeurEcran,  inventaire);
    HandleDecraft(  posX,  posY ,  margeX, margeY ,largeurBouton,hauteurBouton, hauteurEcran ,  largeurEcran, hauteurFenetre, largeurFenetre , textures , inventaire);

}

void EnableFin(){
}