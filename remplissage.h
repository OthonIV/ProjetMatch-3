#ifndef REMPLISSAGE_H
#define REMPLISSAGE_H

#define HAUTEUR 9
#define LARGEUR 9

// Définition de la structure pour le déplacement
typedef struct {
    int x; // Colonne (0 à LARGEUR-1)
    int y; // Ligne (0 à HAUTEUR-1)
} Curseur;

void nettoyerEcran();
void afficherInfos(int niveau, int vies, int coups, int temps);
void remplirPlateau(char plateau[HAUTEUR][LARGEUR]);
void afficherPlateau(char plateau[HAUTEUR][LARGEUR]);
void supprimerAlignements3(char plateau[HAUTEUR][LARGEUR]);
void appliquerGravite(char plateau[HAUTEUR][LARGEUR]);
void echangerSymboles(char plateau[HAUTEUR][LARGEUR], Curseur c1, Curseur c2);

char obtenirBonusLigne(char s);
char obtenirBonusColonne(char s);
void detecterEtCreerBonus(char plateau[HAUTEUR][LARGEUR], Curseur coup);
void activerPouvoir(char plateau[HAUTEUR][LARGEUR], int x, int y);

#endif