#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SpidyLib.h"
#include "CalculLib.h"

// Vérifie si le bouton paramètre est cliqué ou si la touche ESC est pressée.
void CheckOuvertureParametre(Vector2 PosSouris, Rectangle ParaRect, bool *ParametreOuvert) {
    if (CheckMouseCollisionCliked(PosSouris, ParaRect) | IsKeyPressed(KEY_ESCAPE)) {
        *ParametreOuvert = true;
    }
}

// Dessine l'écran des paramètres, avec les boutons et textes.
void DrawParametre(int ScreenWidth, int ScreenHeight, Vector2 Pos1600, Vector2 PosFull, 
                   Vector2 PosRevenir, Vector2 PosQuitter, 
                   int BoutonMenuWidth, int BoutonMenuHeight) {
    
    // Dessine le fond du menu des paramètres
    DrawRectangle(ProportionnelleLargeur(600, ScreenWidth), 
                  ProportionnelleHauteur(300, ScreenHeight), 
                  ProportionnelleLargeur(400, ScreenWidth), 
                  ProportionnelleHauteur(300, ScreenHeight), GREEN);

    // Affiche le texte "Paramètre"
    DrawText("Paramètre", ProportionnelleLargeur(750, ScreenWidth), 
             ProportionnelleHauteur(320, ScreenHeight), 20, BLACK);

    // Dessine les boutons pour les différentes options
    DrawRectangle(Pos1600.x, Pos1600.y, BoutonMenuWidth, BoutonMenuHeight, DARKGRAY);
    DrawText("1600x900", Pos1600.x + ProportionnelleLargeur(15, ScreenWidth), 
             Pos1600.y + ProportionnelleHauteur(10, ScreenHeight), 20, RAYWHITE);

    DrawRectangle(PosFull.x, PosFull.y, BoutonMenuWidth, BoutonMenuHeight, DARKGRAY);
    DrawText("Plein Écran", PosFull.x + ProportionnelleLargeur(15, ScreenWidth), 
             PosFull.y + ProportionnelleHauteur(10, ScreenHeight), 20, RAYWHITE);

    DrawRectangle(PosRevenir.x, PosRevenir.y, BoutonMenuWidth, BoutonMenuHeight, DARKGRAY);
    DrawText("Revenir", PosRevenir.x + ProportionnelleLargeur(15, ScreenWidth), 
             PosRevenir.y + ProportionnelleHauteur(10, ScreenHeight), 20, RAYWHITE);

    DrawRectangle(PosQuitter.x, PosQuitter.y, BoutonMenuWidth, BoutonMenuHeight, RED);
    DrawText("Quitter", PosQuitter.x + ProportionnelleLargeur(15, ScreenWidth), 
             PosQuitter.y + ProportionnelleHauteur(10, ScreenHeight), 20, RAYWHITE);
}

// Vérifie si la souris est cliquée dans un rectangle donné.
bool CheckMouseCollisionCliked(Vector2 PosSouris, Rectangle Rect) {
    return (CheckCollisionPointRec(PosSouris, Rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON));
}

// Gère les interactions avec les boutons des paramètres.
void ChekCollisionParametre(Vector2 PosSouris, Rectangle RetourButtonRect, 
                             Rectangle ButtonRect1600, Rectangle FullButtonRect, 
                             Rectangle QuitterButtonRect, bool *ParametreOuvert, 
                             bool *fullscreen, int *ScreenWidth, int *ScreenHeight) {

    // Si le bouton "Retour" est cliqué, ferme le menu des paramètres.
    if (CheckMouseCollisionCliked(PosSouris, RetourButtonRect)) {
        *ParametreOuvert = false;
    }

    // Si le bouton "1600x900" est cliqué, change la résolution et ferme le menu.
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

    // Si le bouton "Plein Écran" est cliqué, bascule entre plein écran et fenêtre.
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

    // Si le bouton "Quitter" est cliqué, ferme la fenêtre.
    if (CheckMouseCollisionCliked(PosSouris, QuitterButtonRect)) {
        CloseWindow();
    }

    // Si la touche ESC est pressée, ferme le menu des paramètres.
    if (IsKeyPressed(KEY_ESCAPE)) {
        *ParametreOuvert = false;
    }
}