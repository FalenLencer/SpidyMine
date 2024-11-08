#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum {
    COMMUN ,
    RARE ,
    EPIQUE ,
    EVENEMENT ,
    INCASSABLE
} TypeMinerai;

typedef struct Bloc {
   Texture2D Texture;
   Rectangle HitBox;
   int PosX;
   int PosY;
   bool Etat;
   TypeMinerai type;
} Bloc;

typedef struct {
    int Mineraie_Niveau_01;
    int Mineraie_Niveau_02;
    int Mineraie_Niveau_03;
} Inventaire;



Texture2D LoadTextureIfExists(const char *imagePath);
int VerifEtat(Bloc B);
Bloc **NeedGrid(int rows, int cols, Texture blueTexture, Texture yellowTexture, Texture redTexture);
void FreeGrid(Bloc **Grille, int rows);
bool CheckMouseCollisionCliked(Vector2 PosSouris, Rectangle Rect);
void DrawParametre(int ScreenWidth, int ScreenHeight, Vector2 Pos1600, Vector2 PosFull, Vector2 PosRevenir, Vector2 PosQuitter, int BoutonMenuWidth, int BoutonMenuHeight);
void ChekCollisionParametre(Vector2 PosSouris, Rectangle RetourButtonRect, Rectangle ButtonRect1600, Rectangle FullButtonRect, Rectangle QuitterButtonRect, bool *ParametreOuvert, bool *fullscreen, int *ScreenWidth, int *ScreenHeight);

typedef struct {
    Texture2D Minerai_commun;
    Texture2D Minerai_rare;
    Texture2D evenement;
    Texture2D incassable ;
    Texture2D iconParametreTexture;
    Texture2D backgroundTexture;
    Texture2D playerTextureIdle;
    Texture2D playerTextureMove;
    Texture2D playerTextureMoveD;
    Texture2D playerTextureAction;
    Texture2D playerTextureAction2;
    Texture2D playerTextureHaut;
    Texture2D playerTextureMove2;
    Texture2D playerTextureMove2g;
} TexturesJeux;

void InitTextures(TexturesJeux *textures);
void UnloadTextures(TexturesJeux *textures);

void GetMouvements(int Speed, int ScreenWidth, int ScreenHeight, bool *isAction, bool *isMovingRight, bool *isMovingLeft, bool *isMovingHaut, bool *isMovingBas, Vector2 *playerPosition, TexturesJeux textures, Bloc **Grille, int rows, int cols , float echelle);
void DrawMouvements(bool isAction,bool isMovingRight,bool isMovingLeft ,bool isMovingBas,bool isMovingHaut, int frameCounter,Vector2 playerPosition ,TexturesJeux textures, float echelle);
void ActionMiner(bool isAction ,TexturesJeux textures , Vector2 playerPosition );
void SuprCliked(Vector2 PosSouris , Bloc *cube , Inventaire *inventaire);
void CheckOuvertureParametre( Vector2 PosSouris, Rectangle ParaRect ,bool *ParametreOuvert);
void DetecterCollision(Rectangle Personage, Bloc *Cube);
bool IsCollidingWithBloc(Rectangle personnage, Bloc **Grille, int rows, int cols);
void InitInventaire(Inventaire *inventaire);
void DrawInventaire(Inventaire *inventaire, int HauteurLigne);
void DrawLimite(int rows, int cols , int offsetX , int startY , int TailleCarre , int Espace , int ScreenWidth , TexturesJeux textures );