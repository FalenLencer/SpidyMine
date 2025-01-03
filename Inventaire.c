#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SpidyLib.h"
#include "CalculLib.h"

void InitInventaire(Inventaire *inventaire) {
    (*inventaire).Mineraie_Niveau_01=0;
    (*inventaire).Mineraie_Niveau_02=0;
    (*inventaire).Mineraie_Niveau_03=0;
    (*inventaire).Mineraie_Niveau_04=0;
}

void DrawInventaireQuick(Inventaire *inventaire, int hauteurEcran , int largeurEcran) {
    int tailleText= ProportionnelleHauteur(20,hauteurEcran);
    DrawText(TextFormat("Minerai commun : %d",inventaire->Mineraie_Niveau_01), 0,ProportionnelleHauteur(325,hauteurEcran) ,tailleText,GREEN );
    DrawText(TextFormat("Minerai rare : %d",inventaire->Mineraie_Niveau_02), 0,ProportionnelleHauteur(345,hauteurEcran) ,tailleText,BLUE );
    DrawText(TextFormat("Minerai épique : %d",inventaire->Mineraie_Niveau_03), 0,ProportionnelleHauteur(365,hauteurEcran) ,tailleText,PURPLE );
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

void HandleNewPortal( int hauteurEcran , int largeurEcran,int margeX,int margeY ,int hauteurBouton,int largeurBouton, int Tailletext,bool *DessinnerPortail , Inventaire *inventaire){
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
            *DessinnerPortail=true;
            inventaire->Mineraie_Niveau_02 -= prixPortail;

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
            inventaire->Mineraie_Niveau_04 -= prixPortail;
        }
    } else {
        DrawRectangle(posXBoutonAmelioration, posYBoutonAmelioration, largeurBouton, hauteurBouton, RED);
        DrawText("Impossible", posXBoutonAmelioration +ProportionnelleLargeur(10,largeurEcran), posYBoutonAmelioration + ProportionnelleHauteur(10,hauteurEcran), Tailletext, WHITE);
    }
}
    


void HandleUpgrades(int posX , int posY , int margeX ,int margeY, int hauteurEcran ,int largeurEcran, int largeurBouton , int hauteurBouton,bool *DessinnerPortail, bool *IsEnding ,Inventaire *inventaire , Statistiques *stats){
    int dimX=ProportionnelleLargeur(460,largeurEcran);
    int dimY=ProportionnelleHauteur(860,hauteurEcran);
    int tailleText= ProportionnelleHauteur(20,hauteurEcran);
    DrawRectangle(margeX, margeY,dimX , dimY, Fade(DARKBLUE, 0.8f));
    DrawRectangleLinesEx((Rectangle){margeX, margeY, dimX, dimY}, 2, BLACK);
    DrawText("Boutique :", (int) (margeX+(dimX/2)-(MeasureText("Boutique :",tailleText)/2)), margeY + margeY, tailleText, RAYWHITE);

    HandleFortune(  hauteurEcran ,  largeurEcran, margeX, margeY , hauteurBouton, largeurBouton,  tailleText ,  inventaire,  stats);
    HandleVitesseDeplacement( hauteurEcran ,  largeurEcran, margeX, margeY , hauteurBouton, largeurBouton,  tailleText ,  inventaire,  stats);
    HandleNewPortal(  hauteurEcran ,  largeurEcran, margeX, margeY , hauteurBouton, largeurBouton, tailleText , DessinnerPortail , inventaire);
    HandleLastPortal(hauteurEcran ,  largeurEcran, margeX, margeY , hauteurBouton, largeurBouton,  tailleText ,  inventaire,  stats , IsEnding);
}


void DrawCompleteInventory(bool *DessinnerPortail, bool *IsEnding,TexturesJeux textures, Inventaire *inventaire, Statistiques *stats) {
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

    HandleUpgrades(posX , posY , margeX ,margeY, hauteurEcran ,largeurEcran, largeurBouton , hauteurBouton, DessinnerPortail ,IsEnding, inventaire ,stats);
    HandleInventory( posX,  posY ,  margeX, margeY , hauteurEcran ,  largeurEcran, hauteurFenetre, largeurFenetre , textures , inventaire);
    HandleFusion(posX, posY ,  margeX,  margeY , largeurBouton, hauteurBouton,  hauteurFenetre, largeurFenetre , hauteurEcran, largeurEcran,  inventaire);
    HandleDecraft(  posX,  posY ,  margeX, margeY ,largeurBouton,hauteurBouton, hauteurEcran ,  largeurEcran, hauteurFenetre, largeurFenetre , textures , inventaire);

}