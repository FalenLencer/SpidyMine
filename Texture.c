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


void InitTextures(TexturesJeux *textures) {
    (*textures).Minerai_commun = LoadTextureIfExists("Texture_Blocs/Nut.png");
    (*textures).Minerai_rare = LoadTextureIfExists("Texture_Blocs/Minerai1.png");
    (*textures).Minerai_epique = LoadTextureIfExists("Texture_Blocs/Mineraie_epique.PNG");
    (*textures).evenement = LoadTextureIfExists("Texture_Blocs/Evenement.png");
    (*textures).incassable = LoadTextureIfExists("Texture_Blocs/unbreakable.png");
    (*textures).iconParametreTexture = LoadTextureIfExists("parametre.png");
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
    (*textures).Screamer = LoadTextureIfExists("Texture_Blocs/BigSpidy.png");
    (*textures).Talk = LoadTextureIfExists("Texture_Blocs/LittleSpidy.png");
}

void UnloadTextures(TexturesJeux *textures) {
    if ((*textures).Minerai_commun.id > 0) UnloadTexture((*textures).Minerai_commun);
    if ((*textures).Minerai_rare.id > 0) UnloadTexture((*textures).Minerai_rare);
    if ((*textures).Minerai_epique.id > 0) UnloadTexture((*textures).Minerai_epique);
    if ((*textures).evenement.id > 0) UnloadTexture((*textures).evenement); 
    if ((*textures).incassable.id > 0) UnloadTexture((*textures).incassable);
    if ((*textures).iconParametreTexture.id > 0) UnloadTexture((*textures).iconParametreTexture);
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
    if ((*textures).Screamer.id > 0) UnloadTexture((*textures).Screamer);
    if ((*textures).Talk.id > 0) UnloadTexture((*textures).Talk);
}