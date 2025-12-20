#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "affichage_console.h"
#include "remplissage.h"

char symboles[5] = {'X', '&', '+', 'O', '#'}; 
char bonusH[5] = {'1', '2', '3', '4', '5'}; 
char bonusV[5] = {'A', 'B', 'C', 'D', 'E'}; 

void nettoyerEcran() {
    clrscr();
}

void afficherInfos(int niveau, int vies, int coups, int temps) {
    set_color(WHITE, BLACK);
    printf("\n");
    printf(" NIVEAU: %d | VIES: %d | COUPS: %d | TEMPS: %ds\n",
           niveau, vies, coups, temps);
}

void remplirPlateau(char plateau[HAUTEUR][LARGEUR]) {
    for (int i = 0; i < HAUTEUR; i++) {
        for (int j = 0; j < LARGEUR; j++) {
            plateau[i][j] = symboles[rand() % 5];
        }
    }
}

void afficherPlateau(char plateau[HAUTEUR][LARGEUR]) {
    int i, j;
    printf("  ");
    for (j = 0; j < LARGEUR; j++) printf("+---");
    printf("+\n");

    for (i = 0; i < HAUTEUR; i++) {
        printf("  "); 
        for (j = 0; j < LARGEUR; j++) {
            printf("|"); 
            char val = plateau[i][j];

            if (estDeMemeCouleur(val, 'X')) set_color(RED, BLACK);
            else if (estDeMemeCouleur(val, '&')) set_color(CYAN, BLACK);
            else if (estDeMemeCouleur(val, '+')) set_color(GREEN, BLACK);
            else if (estDeMemeCouleur(val, 'O')) set_color(YELLOW, BLACK);
            else if (estDeMemeCouleur(val, '#')) set_color(MAGENTA, BLACK);
            else set_color(WHITE, BLACK);

            if (val >= '1' && val <= '5') printf(" = ");
            else if (val >= 'A' && val <= 'E') printf(" H ");
            else printf(" %c ", val);

            set_color(WHITE, BLACK);
        }
        printf("|\n");
        printf("  ");
        for (j = 0; j < LARGEUR; j++) printf("+---");
        printf("+\n");
    }
}

int estDeMemeCouleur(char a, char b) {
    if (a == ' ' || b == ' ') return 0;
    
    char originaux[] = {'X', '&', '+', 'O', '#'};
    char bonusH[]    = {'1', '2', '3', '4', '5'};
    char bonusV[]    = {'A', 'B', 'C', 'D', 'E'};

    for (int i = 0; i < 5; i++) {
        int aOk = (a == originaux[i] || a == bonusH[i] || a == bonusV[i]);
        int bOk = (b == originaux[i] || b == bonusH[i] || b == bonusV[i]);
        if (aOk && bOk) return 1;
    }
    return 0;
}


void supprimerAlignements3(char plateau[HAUTEUR][LARGEUR], int collectes[5], int *vies) {
    int aSupprimer[HAUTEUR][LARGEUR] = {0}; 
    int i, j, k, m;

    // Recherche des alignements de BONUS horizontaux
    for (i = 0; i < HAUTEUR; i++) {
        for (j = 0; j <= LARGEUR - 3; j++) {
            int countBonus = 0;
            for(int b = 0; b < 3; b++) {
                char c = plateau[i][j+b];
                if ((c >= '1' && c <= '5') || (c >= 'A' && c <= 'E')) countBonus++;
            }
            if (countBonus == 3) {
                (*vies)++; // Gain d'une vie
                for(int b = 0; b < 3; b++) aSupprimer[i][j+b] = 1;
            }
        }
    }

    // Recherche des alignements de BONUS verticaux
    for (j = 0; j < LARGEUR; j++) {
        for (i = 0; i <= HAUTEUR - 3; i++) {
            int countBonus = 0;
            for(int b = 0; b < 3; b++) {
                char c = plateau[i+b][j];
                if ((c >= '1' && c <= '5') || (c >= 'A' && c <= 'E')) countBonus++;
            }
            if (countBonus == 3) {
                (*vies)++; // Gain d'une vie
                for(int b = 0; b < 3; b++) aSupprimer[i+b][j] = 1;
            }
        }
    }

    // Détection Croix de 3x3
    for (i = 1; i < HAUTEUR - 1; i++) {
        for (j = 1; j < LARGEUR - 1; j++) {
            char ref = plateau[i][j];
            if (ref == ' ') continue;

            int estCroix = 1;
            for(int dy = -1; dy <= 1; dy++) {
                for(int dx = -1; dx <= 1; dx++) {
                    if (!estDeMemeCouleur(ref, plateau[i+dy][j+dx])) { estCroix = 0; break; }
                }
                if(!estCroix) break;
            }

            if (estCroix) {
                for(int c = 0; c < LARGEUR; c++) aSupprimer[i][c] = 1;
                for(int l = 0; l < HAUTEUR; l++) aSupprimer[l][j] = 1;
            }
        }
    }

    // Détection Carré de 4x4
    for (i = 0; i <= HAUTEUR - 4; i++) {
        for (j = 0; j <= LARGEUR - 4; j++) {
            char ref = plateau[i][j];
            if (ref == ' ') continue;

            int estCarre = 1;
            for(int dy = 0; dy < 4; dy++) {
                for(int dx = 0; dx < 4; dx++) {
                    if (!estDeMemeCouleur(ref, plateau[i+dy][j+dx])) { estCarre = 0; break; }
                }
                if(!estCarre) break;
            }

            if (estCarre) {
                for(int dy = 0; dy < 4; dy++) {
                    for(int dx = 0; dx < 4; dx++) aSupprimer[i+dy][j+dx] = 1;
                }
            }
        }
    }

    // Alignement Horizontal
    for (i = 0; i < HAUTEUR; i++) {
        for (j = 0; j <= LARGEUR - 3; ) {
            char ref = plateau[i][j];
            if (ref == ' ') { j++; continue; }

            int count = 1;
            while (j + count < LARGEUR && estDeMemeCouleur(ref, plateau[i][j + count])) count++;

            if (count >= 3) {
                if (count >= 6) { 
                    // Suite de 6 = supprimer TOUS les items de cette couleur
                    for(int l=0; l<HAUTEUR; l++) {
                        for(int c=0; c<LARGEUR; c++) {
                            if(estDeMemeCouleur(ref, plateau[l][c])) aSupprimer[l][c] = 1;
                        }
                    }
                } else {
                    for (k = 0; k < count; k++) aSupprimer[i][j + k] = 1;
                    if (count == 4 || count == 5) {
                        for (m = 0; m < 5; m++) {
                            if (estDeMemeCouleur(ref, symboles[m])) {
                                plateau[i][j] = bonusH[m]; 
                                aSupprimer[i][j] = 0; 
                                break;
                            }
                        }
                    }
                }
                j += count;
            } else j++;
        }
    }

    // Alignement Vertical
    for (j = 0; j < LARGEUR; j++) {
        for (i = 0; i <= HAUTEUR - 3; ) {
            char ref = plateau[i][j];
            if (ref == ' ') { i++; continue; }

            int count = 1;
            while (i + count < HAUTEUR && estDeMemeCouleur(ref, plateau[i + count][j])) count++;

            if (count >= 3) {
                if (count >= 6) {
                    for(int l=0; l<HAUTEUR; l++) {
                        for(int c=0; c<LARGEUR; c++) {
                            if(estDeMemeCouleur(ref, plateau[l][c])) aSupprimer[l][c] = 1;
                        }
                    }
                } else {
                    for (k = 0; k < count; k++) aSupprimer[i + k][j] = 1;
                    if (count == 4 || count == 5) {
                        for (m = 0; m < 5; m++) {
                            if (estDeMemeCouleur(ref, symboles[m])) {
                                plateau[i][j] = bonusV[m];
                                aSupprimer[i][j] = 0;
                                break;
                            }
                        }
                    }
                }
                i += count;
            } else i++;
        }
    }

    // Activation des bonus
    int changement;
    do {
        changement = 0;
        for (i = 0; i < HAUTEUR; i++) {
            for (j = 0; j < LARGEUR; j++) {
                if (aSupprimer[i][j] == 1) {
                    for (m = 0; m < 5; m++) {
                        if (plateau[i][j] == bonusH[m]) {
                            for (int c = 0; c < LARGEUR; c++) {
                                if (aSupprimer[i][c] == 0) { aSupprimer[i][c] = 1; changement = 1; }
                            }
                        }
                        if (plateau[i][j] == bonusV[m]) {
                            for (int l = 0; l < HAUTEUR; l++) {
                                if (aSupprimer[l][j] == 0) { aSupprimer[l][j] = 1; changement = 1; }
                            }
                        }
                    }
                }
            }
        }
    } while (changement);

    // Suppression et comptage
    for (i = 0; i < HAUTEUR; i++) {
        for (j = 0; j < LARGEUR; j++) {
            if (aSupprimer[i][j] == 1) {
                for(int m=0; m<5; m++) {
                    if(estDeMemeCouleur(plateau[i][j], symboles[m])) {
                        collectes[m]++;
                        break;
                    }
                }
                plateau[i][j] = ' ';
            }
        }
    }
}

void appliquerGravite(char plateau[HAUTEUR][LARGEUR]) {
    for (int j = 0; j < LARGEUR; j++) {
        int vide = HAUTEUR - 1;
        for (int i = HAUTEUR - 1; i >= 0; i--) {
            if (plateau[i][j] != ' ') {
                char temp = plateau[i][j];
                plateau[i][j] = ' ';
                plateau[vide][j] = temp;
                vide--;
            }
        }
        for (int i = vide; i >= 0; i--) {
            plateau[i][j] = symboles[rand() % 5];
        }
    }
}

void echangerSymboles(char plateau[HAUTEUR][LARGEUR], Curseur c1, Curseur c2) {
    char temp = plateau[c1.y][c1.x];
    plateau[c1.y][c1.x] = plateau[c2.y][c2.x];
    plateau[c2.y][c2.x] = temp;
}

int detecterAlignement(char plateau[HAUTEUR][LARGEUR]) {
    for (int i = 0; i < HAUTEUR; i++) {
        for (int j = 0; j < LARGEUR; j++) {
            char ref = plateau[i][j];
            if (ref == ' ') continue;
            if (j <= LARGEUR - 3 && estDeMemeCouleur(ref, plateau[i][j+1]) && estDeMemeCouleur(ref, plateau[i][j+2]))
                return 1;
            if (i <= HAUTEUR - 3 && estDeMemeCouleur(ref, plateau[i+1][j]) && estDeMemeCouleur(ref, plateau[i+2][j]))
                return 1;
        }
    }
    return 0;
}