#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "affichage_console.h"
#include "remplissage.h"

// Prototype de la nouvelle fonction de test
void injecter3Bonus(char plateau[HAUTEUR][LARGEUR]);

// Prototypes existants
void injecterLigne4H(char plateau[HAUTEUR][LARGEUR]);
void injecterLigne4V(char plateau[HAUTEUR][LARGEUR]);
void injecterLigne6(char plateau[HAUTEUR][LARGEUR]);
void injecterCarre4x4(char plateau[HAUTEUR][LARGEUR]);
void injecterCroix5x5(char plateau[HAUTEUR][LARGEUR]);

extern char symboles[5];

int main() {
    char plateau[HAUTEUR][LARGEUR];
    int collectes[5] = {0, 0, 0, 0, 0}; 
    int viesDeTest = 3; // Variable pour tester le gain de vie
    
    srand(time(NULL));
    hide_cursor();

    int choixTest = 0;

    do {
        nettoyerEcran();
        set_color(CYAN, BLACK);
        printf("\n\t--- LABO DE TESTS : DESTRUCTION DE PATTERNS ---\n");
        set_color(GREEN, BLACK);
        printf("\t          VIES ACTUELLES : %d\n\n", viesDeTest); // Affichage des vies
        
        set_color(LIGHTCYAN, BLACK);
        printf(" 1. Ligne de 4 Horizontale.\n");          
        printf(" 2. Ligne de 4 Verticale.\n");
        printf(" 3. Ligne de 6\n");
        printf(" 4. Carre 4x4\n");
        printf(" 5. Croix 5x5\n");
        set_color(LIGHTCYAN, BLACK);
        printf(" 6. Aligner 3 Bonus -> +1 Vie\n");
        set_color(LIGHTRED, BLACK);
        printf(" 7. Quitter\n\n");
        set_color(WHITE, BLACK);
        printf(" Votre choix : ");

        choixTest = getch() - '0';

        if (choixTest >= 1 && choixTest <= 6) {
            remplirPlateau(plateau);
            viesDeTest = 3; // Réinitialiser les vies avant chaque test

            // 1. Injection
            switch(choixTest) {
                case 1: injecterLigne4H(plateau); break;
                case 2: injecterLigne4V(plateau); break;
                case 3: injecterLigne6(plateau); break;
                case 4: injecterCarre4x4(plateau); break;
                case 5: injecterCroix5x5(plateau); break;
                case 6: injecter3Bonus(plateau); break; // Injection des bonus
            }

            // 2. Premier affichage
            afficherPlateau(plateau); 
            set_color(YELLOW, BLACK);
            printf("\n [ETAPE 1] Test injecte. Appuyez pour SUPPRIMER...");
            getch();

            // 3. Appel avec le pointeur sur viesDeTest
            supprimerAlignements3(plateau, collectes, &viesDeTest); 

            // 4. Deuxième affichage
            afficherPlateau(plateau);
            set_color(LIGHTGREEN, BLACK);
            printf("\n [ETAPE 2] Items supprimes. Vies : %d. Appuyez pour GRAVITE...", viesDeTest);
            getch();

            // 5. Gravité
            appliquerGravite(plateau);
            afficherPlateau(plateau);
            printf("\n [ETAPE 3] Fini. Appuyez pour revenir au menu.");
            getch();
        }

    } while (choixTest != 7);

    show_cursor();
    return 0;
}


void injecter3Bonus(char plateau[HAUTEUR][LARGEUR]) {
    plateau[4][3] = '1'; // Bonus '=' rouge
    plateau[4][4] = 'B'; // Bonus 'H' bleu
    plateau[4][5] = '3'; // Bonus '=' vert
}

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