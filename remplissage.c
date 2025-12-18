#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "affichage_console.h"
#define HAUTEUR 9
#define LARGEUR 9

char symboles[5] = {'X', '&', '+', 'O', '#'};

// Structure pour les positions
typedef struct {
    int x; // Colonne
    int y; // Ligne
} Curseur;

// Prototypes
void echangerSymboles(char plateau[HAUTEUR][LARGEUR], Curseur c1, Curseur c2);
int memeFamille(char a, char b);
void activerPouvoir(char plateau[HAUTEUR][LARGEUR], int x, int y);

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
    for (int i = 0; i < HAUTEUR; i++) {
        printf("  ");
        for (int j = 0; j < LARGEUR; j++) printf("+---");
        printf("+\n  ");
        for (int j = 0; j < LARGEUR; j++) {
            char val = plateau[i][j];
            printf("|");
            // Couleurs basées sur le caractère
            switch (val) {
                case 'X': case 'x': case '1': set_color(RED, BLACK); break;
                case '&': case 'a': case '2': set_color(CYAN, BLACK); break;
                case '+': case 'p': case '3': set_color(GREEN, BLACK); break;
                case 'O': case 'o': case '4': set_color(YELLOW, BLACK); break;
                case '#': case 'm': case '5': set_color(MAGENTA, BLACK); break;
                default:  set_color(WHITE, BLACK);
            }
            // Affichage du bonus
            if (val >= '1' && val <= '5') printf(" H "); // Bonus Colonne
            else if (val == 'x' || val == 'a' || val == 'p' || val == 'o' || val == 'm') printf(" = "); // Bonus Ligne
            else if (val == ' ') printf("   ");
            else printf(" %c ", val);
            set_color(WHITE, BLACK);
        }
        printf("|\n");
    }
    printf("  ");
    for (int j = 0; j < LARGEUR; j++) printf("+---");
    printf("+\n");
}


void supprimerAlignements3(char plateau[HAUTEUR][LARGEUR]) {
    int i, j;
    int aSupprimer[HAUTEUR][LARGEUR] = {0}; // 1 si on doit supprimer, 0 sinon

    // Détection des alignements de 3 (Horizontaux)
    for (i = 0; i < HAUTEUR; i++) {
        for (j = 0; j < LARGEUR - 2; j++) {
            if (plateau[i][j] != ' ' && 
                memeFamille(plateau[i][j], plateau[i][j+1]) && 
                memeFamille(plateau[i][j], plateau[i][j+2])) {
                aSupprimer[i][j] = 1;
                aSupprimer[i][j+1] = 1;
                aSupprimer[i][j+2] = 1;
            }
        }
    }

    // Détection des alignements de 3 (Verticaux)
    for (j = 0; j < LARGEUR; j++) {
        for (i = 0; i < HAUTEUR - 2; i++) {
            if (plateau[i][j] != ' ' && 
                memeFamille(plateau[i][j], plateau[i+1][j]) && 
                memeFamille(plateau[i][j], plateau[i+2][j])) {
                aSupprimer[i][j] = 1;
                aSupprimer[i+1][j] = 1;
                aSupprimer[i+2][j] = 1;
            }
        }
    }

    // Application des pouvoirs et suppression
    for (i = 0; i < HAUTEUR; i++) {
        for (j = 0; j < LARGEUR; j++) {
            if (aSupprimer[i][j] == 1) {
                // Si la case qu'on va supprimer est un bonus, on active son pouvoir AVANT
                activerPouvoir(plateau, j, i); 
                plateau[i][j] = ' '; // On vide la case
            }
        }
    }
}

void appliquerGravite(char plateau[HAUTEUR][LARGEUR]) {
    int colonne, ligne;
    int espaceTrouve;

    do {
        espaceTrouve = 0;

        for (colonne = 0; colonne < LARGEUR; colonne++) {

            // Parcours du bas vers le haut
            for (ligne = HAUTEUR - 1; ligne >= 0; ligne--) {

                if (plateau[ligne][colonne] == ' ') {

                    // Descente d'UNE ligne
                    for (int k = ligne; k > 0; k--) {
                        plateau[k][colonne] = plateau[k - 1][colonne];
                    }

                    // Génération en haut
                    plateau[0][colonne] = symboles[rand() % 5];
                    espaceTrouve = 1;
                    break; // UNE SEULE suppression par colonne
                }
            }
        }
        supprimerAlignements3(plateau); // Pour enlever les alignements créés
    } while (espaceTrouve); // Recommence tant qu'il reste des espaces
}


void echangerSymboles(char plateau[HAUTEUR][LARGEUR], Curseur c1, Curseur c2) {
    char temp = plateau[c1.y][c1.x];
    plateau[c1.y][c1.x] = plateau[c2.y][c2.x];
    plateau[c2.y][c2.x] = temp;
}


int memeFamille(char a, char b) {
    if (a == ' ' || b == ' ') return 0;
    // On transforme tout en symbole de base pour comparer
    char baseA = a, baseB = b;
    if (a == 'x' || a == '1') baseA = 'X';
    if (b == 'x' || b == '1') baseB = 'X';
    if (a == 'a' || a == '2') baseA = '&';
    if (b == 'a' || b == '2') baseB = '&';
    if (a == 'p' || a == '3') baseA = '+';
    if (b == 'p' || b == '3') baseB = '+';
    if (a == 'o' || a == '4') baseA = 'O';
    if (b == 'o' || b == '4') baseB = 'O';
    if (a == 'm' || a == '5') baseA = '#';
    if (b == 'm' || b == '5') baseB = '#';
    
    return (baseA == baseB);
}


// BONUS
// Table de conversion pour transformer un symbole de base en bonus
char obtenirBonusLigne(char s) {
    if (s == 'X') return 'x'; if (s == '&') return 'a'; if (s == '+') return 'p';
    if (s == 'O') return 'o'; if (s == '#') return 'm';
    return s;
}


char obtenirBonusColonne(char s) {
    if (s == 'X') return '1'; if (s == '&') return '2'; if (s == '+') return '3';
    if (s == 'O') return '4'; if (s == '#') return '5';
    return s;
}


void detecterEtCreerBonus(char plateau[HAUTEUR][LARGEUR], Curseur coup) {
    char s = plateau[coup.y][coup.x];
    if (s == ' ') return;

    // Vérification horizontale
    int countH = 1, gauche = coup.x - 1, droite = coup.x + 1;
    while (gauche >= 0 && plateau[coup.y][gauche] == s) { countH++; gauche--; }
    while (droite < LARGEUR && plateau[coup.y][droite] == s) { countH++; droite++; }

    // Vérification verticale
    int countV = 1, haut = coup.y - 1, bas = coup.y + 1;
    while (haut >= 0 && plateau[haut][coup.x] == s) { countV++; haut--; }
    while (bas < HAUTEUR && plateau[bas][coup.x] == s) { countV++; bas++; }

    if (countH >= 4) plateau[coup.y][coup.x] = obtenirBonusLigne(s);
    else if (countV >= 4) plateau[coup.y][coup.x] = obtenirBonusColonne(s);
}


void activerPouvoir(char plateau[HAUTEUR][LARGEUR], int x, int y) {
    char c = plateau[y][x];
    if (c == ' ') return;
    // Si bonus '=' (ligne)
    if (c == 'x' || c == 'a' || c == 'p' || c == 'o' || c == 'm') {
        for (int j = 0; j < LARGEUR; j++) plateau[y][j] = ' ';
    }
    // Si bonus 'H' (colonne)
    else if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5') {
        for (int i = 0; i < HAUTEUR; i++) plateau[i][x] = ' ';
    }
}