#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "affichage_console.h"
#include "remplissage.h"

// Prototypes des fonctions d'injection
void injecterLigne4H(char plateau[HAUTEUR][LARGEUR]);
void injecterLigne4V(char plateau[HAUTEUR][LARGEUR]);
void injecterLigne6(char plateau[HAUTEUR][LARGEUR]);
void injecterCarre4x4(char plateau[HAUTEUR][LARGEUR]);
void injecterCroix5x5(char plateau[HAUTEUR][LARGEUR]);

extern char symboles[5];

int main() {
    char plateau[HAUTEUR][LARGEUR];
    int collectes[5] = {0, 0, 0, 0, 0}; // Pour simuler la récolte d'items
    srand(time(NULL));
    hide_cursor();

    int choixTest = 0;

    do {
        nettoyerEcran();
        set_color(CYAN, BLACK);
        printf("\n\t--- LABO DE TESTS : DESTRUCTION DE PATTERNS ---\n\n");
        set_color(LIGHTCYAN, BLACK);
        printf(" 1. Ligne de 4 Horizontale.\n");          
        printf(" 2. Ligne de 4 Verticale.\n");
        printf(" 3. Ligne de 6\n");
        printf(" 4. Carre 4x4\n");
        printf(" 5. Croix 5x5\n");
        printf(" 6. Quitter\n\n");
        printf(" Votre choix : ");

        choixTest = getch() - '0';

        if (choixTest >= 1 && choixTest <= 5) {
            remplirPlateau(plateau); 

            // 1. Injection
            switch(choixTest) {
                case 1: injecterLigne4H(plateau); break;
                case 2: injecterLigne4V(plateau); break;
                case 3: injecterLigne6(plateau); break;
                case 4: injecterCarre4x4(plateau); break;
                case 5: injecterCroix5x5(plateau); break;
            }

            // 2. Premier affichage (Avant suppression)
            afficherPlateau(plateau); 
            set_color(YELLOW, BLACK);
            printf("\n [ETAPE 1] Test injecte. Appuyez sur une touche pour SUPPRIMER...");
            getch();

            // 3. Appel de la fonction de suppression
            supprimerAlignements3(plateau, collectes);

            // 4. Deuxième affichage (Après suppression)
            afficherPlateau(plateau);
            set_color(LIGHTGREEN, BLACK);
            printf("\n [ETAPE 2] Items supprimes. Appuyez pour la GRAVITE...");
            getch();

            // 5. Application de la gravité
            appliquerGravite(plateau);
            afficherPlateau(plateau);
            printf("\n [ETAPE 3] Gravite appliquee. Appuyez pour revenir au menu.");
            getch();
        }

    } while (choixTest != 6);

    show_cursor();
    return 0;
}

// --- Garde tes fonctions d'injection en dessous ---
void injecterLigne4H(char plateau[HAUTEUR][LARGEUR]) {
    for(int j = 2; j < 6; j++) plateau[4][j] = symboles[0];
}
void injecterLigne4V(char plateau[HAUTEUR][LARGEUR]) {
    for(int i = 2; i < 6; i++) plateau[i][4] = symboles[1];
}
void injecterLigne6(char plateau[HAUTEUR][LARGEUR]) {
    for(int j = 1; j < 7; j++) plateau[3][j] = symboles[2];
}
void injecterCarre4x4(char plateau[HAUTEUR][LARGEUR]) {
    for(int i = 2; i < 6; i++) 
        for(int j = 2; j < 6; j++) plateau[i][j] = symboles[3];
}
void injecterCroix5x5(char plateau[HAUTEUR][LARGEUR]) {
    int cX = 4, cY = 4;
    for(int j = cX-2; j <= cX+2; j++) plateau[cY][j] = symboles[4];
    for(int i = cY-2; i <= cY+2; i++) plateau[i][cX] = symboles[4];
}