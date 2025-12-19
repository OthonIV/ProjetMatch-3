#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "affichage_console.h"
#define HAUTEUR 9
#define LARGEUR 9

char symboles[5] = {'X', '&', '+', 'O', '#'}; // Symboles de base
char bonusH[5] = {'1', '2', '3', '4', '5'}; // Bonus Horizontal (=)
char bonusV[5] = {'A', 'B', 'C', 'D', 'E'}; // Bonus Vertical (H)

// Structure pour les positions
typedef struct {
    int x; // Colonne
    int y; // Ligne
} Curseur;

// Prototype
int estDeMemeCouleur(char a, char b);
void echangerSymboles(char plateau[HAUTEUR][LARGEUR], Curseur c1, Curseur c2);


void nettoyerEcran() {
    clrscr();
}


void afficherInfos(int niveau, int vies, int coups, int temps) {
    set_color(WHITE, BLACK);
    printf("\n");
    printf(" NIVEAU: %d | VIES: %d | COUPS: %d | TEMPS: %ds\n",
           niveau, vies, coups, temps);
}

// Debut de partie
void remplirPlateau(char plateau[HAUTEUR][LARGEUR]) {
    for (int i = 0; i < HAUTEUR; i++) {
        for (int j = 0; j < LARGEUR; j++) {
            plateau[i][j] = symboles[rand() % 5];
        }
    }
}


void afficherPlateau(char plateau[HAUTEUR][LARGEUR]) {
    nettoyerEcran();
    int i, j;

    // Afficher la bordure du haut
    printf("  ");
    for (j = 0; j < LARGEUR; j++) printf("+---");
    printf("+\n");

    for (i = 0; i < HAUTEUR; i++) {
        printf("  "); // Marge à gauche
        for (j = 0; j < LARGEUR; j++) {
            printf("|"); // Bordure gauche de chaque case
            
            char val = plateau[i][j];

            // Couleur
            if (estDeMemeCouleur(val, 'X')) set_color(RED, BLACK);
            else if (estDeMemeCouleur(val, '&')) set_color(CYAN, BLACK);
            else if (estDeMemeCouleur(val, '+')) set_color(GREEN, BLACK);
            else if (estDeMemeCouleur(val, 'O')) set_color(YELLOW, BLACK);
            else if (estDeMemeCouleur(val, '#')) set_color(MAGENTA, BLACK);
            else set_color(WHITE, BLACK); // Couleur par défaut (ex: cases vides)

            // Symbole
            if (val == '1' || val == '2' || val == '3' || val == '4' || val == '5') 
                printf(" = ");
            else if (val == 'A' || val == 'B' || val == 'C' || val == 'D' || val == 'E') 
                printf(" H ");
            else 
                printf(" %c ", val);

            set_color(WHITE, BLACK); // On remet en blanc pour les bordures "|"
        }
        printf("|\n"); // Fin de la ligne du plateau

        // Afficher la ligne de séparation entre chaque rangée
        printf("  ");
        for (j = 0; j < LARGEUR; j++) printf("+---");
        printf("+\n");
    }
}


// Vérifie si deux symboles appartiennent à la même famille de couleur
int estDeMemeCouleur(char a, char b) {
    if (a == ' ' || b == ' ') return 0;
    
    char originaux[] = {'X', '&', '+', 'O', '#'};
    char bonusH[]    = {'1', '2', '3', '4', '5'};
    char bonusV[]    = {'A', 'B', 'C', 'D', 'E'};

    for (int i = 0; i < 5; i++) {
        // On vérifie si 'a' ET 'b' sont l'un des trois types de la couleur i
        int aOk = (a == originaux[i] || a == bonusH[i] || a == bonusV[i]);
        int bOk = (b == originaux[i] || b == bonusH[i] || b == bonusV[i]);
        if (aOk && bOk) return 1;
    }
    return 0;
}


void supprimerAlignements3(char plateau[HAUTEUR][LARGEUR]) {
    // 0: rien, 1: à supprimer (pion normal ou bonus)
    int aSupprimer[HAUTEUR][LARGEUR] = {0}; 
    int i, j, k, m;

    // Recherche horizontale
    for (i = 0; i < HAUTEUR; i++) {
        for (j = 0; j <= LARGEUR - 3; ) {
            char ref = plateau[i][j];
            if (ref == ' ') { j++; continue; }

            int count = 1;
            while (j + count < LARGEUR && estDeMemeCouleur(ref, plateau[i][j + count])) {
                count++;
            }

            if (count >= 3) {
                // On marque tout pour la suppression
                for (k = 0; k < count; k++) {
                    int col = j + k;
                    aSupprimer[i][col] = 1;
                    
                    // Si on rencontre un bonus déjà présent, on active son effet
                    for (m = 0; m < 5; m++) {
                        if (plateau[i][col] == bonusH[m]) { // Bonus Ligne (=)
                            for (int c = 0; c < LARGEUR; c++) aSupprimer[i][c] = 1;
                        }
                        if (plateau[i][col] == bonusV[m]) { // Bonus Colonne (H)
                            for (int l = 0; l < HAUTEUR; l++) aSupprimer[l][col] = 1;
                        }
                    }
                }

                // Création du nouveau bonus si alignement de 4 ou plus
                if (count >= 4) {
                    char nouveauBonus = ' ';
                    // On cherche quelle est la couleur de base pour donner le bon bonus
                    for (m = 0; m < 5; m++) {
                        if (estDeMemeCouleur(ref, symboles[m])) {
                            nouveauBonus = bonusH[m]; // On crée un '='
                        }
                    }
                    // On place le bonus au début de l'alignement
                    if (nouveauBonus != ' ') {
                        plateau[i][j] = nouveauBonus; 
                        aSupprimer[i][j] = 0; // On sauve cette case de la suppression !
                    }
                }

                j += count;
            } else j++;
        }
    }

    // Recherche verticale
    for (j = 0; j < LARGEUR; j++) {
        for (i = 0; i <= HAUTEUR - 3; ) {
            char ref = plateau[i][j];
            if (ref == ' ') { i++; continue; }

            int count = 1;
            while (i + count < HAUTEUR && estDeMemeCouleur(ref, plateau[i + count][j])) {
                count++;
            }

            if (count >= 3) {
                // On marque tout pour la suppression
                for (k = 0; k < count; k++) {
                    int lig = i + k;
                    aSupprimer[lig][j] = 1;

                    // Si on rencontre un bonus déjà présent, on active son effet
                    for (m = 0; m < 5; m++) {
                        if (plateau[lig][j] == bonusV[m]) { // Bonus Colonne (H)
                            for (int l = 0; l < HAUTEUR; l++) aSupprimer[l][j] = 1;
                        }
                        if (plateau[lig][j] == bonusH[m]) { // Bonus Ligne (=)
                            for (int c = 0; c < LARGEUR; c++) aSupprimer[lig][c] = 1;
                        }
                    }
                }

                // Création du nouveau bonus si alignement de 4 ou plus
                if (count >= 4) {
                    char nouveauBonus = ' ';
                    for (m = 0; m < 5; m++) {
                        if (estDeMemeCouleur(ref, symboles[m])) {
                            nouveauBonus = bonusV[m]; // On crée un 'H'
                        }
                    }
                    if (nouveauBonus != ' ') {
                        plateau[i][j] = nouveauBonus;
                        aSupprimer[i][j] = 0; // On sauve cette case !
                    }
                }

                i += count;
            } else i++;
        }
    }

    // Suppression effective des symboles marqués
    for (i = 0; i < HAUTEUR; i++) {
        for (j = 0; j < LARGEUR; j++) {
            if (aSupprimer[i][j] == 1) {
                plateau[i][j] = ' ';
            }
        }
    }
}

void appliquerGravite(char plateau[HAUTEUR][LARGEUR]) {
    for (int j = 0; j < LARGEUR; j++) {
        int vide = HAUTEUR - 1;
        // On fait descendre les caractères existants
        for (int i = HAUTEUR - 1; i >= 0; i--) {
            if (plateau[i][j] != ' ') {
                char temp = plateau[i][j];
                plateau[i][j] = ' ';
                plateau[vide][j] = temp;
                vide--;
            }
        }
        // On remplit le haut avec de nouveaux caractères normaux
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

// Juste pour revenir en arrière si l'action n'a pas créé d'alignement. C'est pas opti? Oui; mais ça marche.
int detecterAlignement(char plateau[HAUTEUR][LARGEUR]) {
    for (int i = 0; i < HAUTEUR; i++) {
        for (int j = 0; j < LARGEUR; j++) {
            char ref = plateau[i][j];
            if (ref == ' ') continue;
            // Test horizontal
            if (j <= LARGEUR - 3 && estDeMemeCouleur(ref, plateau[i][j+1]) && estDeMemeCouleur(ref, plateau[i][j+2]))
                return 1;
            // Test vertical
            if (i <= HAUTEUR - 3 && estDeMemeCouleur(ref, plateau[i+1][j]) && estDeMemeCouleur(ref, plateau[i+2][j]))
                return 1;
        }
    }
    return 0;
}