#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Bloc
{
   Texture2D Texture;
   Rectangle HitBox;
   int PosX;
   int PosY;
   bool Etat;
   int Type;
}Bloc;

Texture2D LoadTextureIfExists(const char *imagePath);
int VerifEtat(Bloc B) ;
Bloc ** NeedGrid(int rows , int cols ,Texture blueTexture,Texture yellowTexture, Texture redTexture);
void FreeGrid(Bloc **Grille, int rows);
bool CheckMouseCollisionCliked(Vector2 PosSouris,Rectangle Rect);