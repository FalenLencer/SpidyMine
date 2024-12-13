#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum {
    COMMUN ,
    RARE ,
    EPIQUE ,
    LEGENDAIRE ,
    EVENEMENT ,
    INCASSABLE
} TypeMinerai;

typedef struct Bloc {
   Texture2D Texture;
   Rectangle HitBox;
   int PosX;
   int PosY;
   bool Etat;
   bool PeutMiner;
   TypeMinerai type;
} Bloc;

typedef struct {
    int Mineraie_Niveau_01;
    int Mineraie_Niveau_02;
    int Mineraie_Niveau_03;
    int Mineraie_Niveau_04;
} Inventaire;

void InitInventaire(Inventaire *inventaire);


typedef struct 
{
    int Fortune;
    int Vitesse;
    int Niveau;
}Statistiques;

void InitStats(Statistiques *stats);

typedef struct {
    Texture2D Minerai_commun;
    Texture2D Minerai_rare;
    Texture2D Minerai_epique;
    Texture2D evenement;
    Texture2D incassable ;
    Texture2D iconParametreTexture;
    Texture2D backgroundTexture;
    Texture2D playerTextureIdle;
    Texture2D playerTextureMoveDroite1;
    Texture2D playerTextureMoveDroite2;
    Texture2D playerTextureAction;
    Texture2D playerTextureAction2;
    Texture2D playerTextureHaut;
    Texture2D playerTextureHaut2;
    Texture2D playerTextureBas;
    Texture2D playerTextureBas2;
    Texture2D playerTextureMoveGauche1;
    Texture2D playerTextureMoveGauche2;
    Texture2D PortailNewmine;
    Texture2D PortailFin;
} TexturesJeux;

Texture2D LoadTextureIfExists(const char *imagePath);
void InitTextures(TexturesJeux *textures);
void UnloadTextures(TexturesJeux *textures);


float ProportionnelleLargeur(float valeur, int ScreenWidth);
float ProportionnelleHauteur(float valeur, int ScreenHeight);

int VerifEtat(Bloc B);


Bloc **NeedGrid(int rows, int cols, int additionalCols, Texture2D Minerais[], Texture incassable,Texture evenement, TypeMinerai Types[],int NUM_MINERAIS);
void FreeGrid(Bloc **Grille, int rows);


bool CheckMouseCollisionCliked(Vector2 PosSouris, Rectangle Rect);
void ChekCollisionParametre(Vector2 PosSouris, Rectangle RetourButtonRect, Rectangle ButtonRect1600, Rectangle FullButtonRect, Rectangle QuitterButtonRect, bool *ParametreOuvert, bool *fullscreen, int *ScreenWidth, int *ScreenHeight);
void CheckOuvertureInventaire(bool *InventaireOuvert);
void CheckOuvertureParametre( Vector2 PosSouris, Rectangle ParaRect ,bool *ParametreOuvert);


bool IsCollidingWithBloc(Rectangle personnage, Bloc ***Grille, int rows, int cols , int additionalCols);
void GetMouvements(int Speed, int ScreenWidth, int ScreenHeight, bool *isAction, bool *isMovingRight, bool *isMovingLeft, bool *isMovingHaut, bool *isMovingBas, Vector2 *playerPosition, TexturesJeux textures, Bloc ***Grille, int rows, int cols,int additionalCols , float echelle);
void DrawMouvements(bool isAction,bool isMovingRight,bool isMovingLeft ,bool isMovingBas,bool isMovingHaut, int frameCounter,Vector2 playerPosition ,TexturesJeux textures, float echelle);

void DrawParametre(int ScreenWidth, int ScreenHeight, Vector2 Pos1600, Vector2 PosFull, Vector2 PosRevenir, Vector2 PosQuitter, int BoutonMenuWidth, int BoutonMenuHeight);
void DrawInventaireQuick(Inventaire *inventaire, int hauteurEcran , int largeurEcran);
void DrawCompleteInventory(int rows, int cols, int additionalCols, int NUM_MINERAIS, bool *IsEnding, TexturesJeux textures, Inventaire *inventaire, Statistiques *stats, Bloc ***Grille);


void SuprCliked(Vector2 PosSouris , Bloc *cube , Inventaire *inventaire,Statistiques stats);
void DetecterCollision(Rectangle Personage, Bloc *Cube);

void EnableFin();