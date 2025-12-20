#ifndef REMPLISSAGE_H
#define REMPLISSAGE_H

#define HAUTEUR 9
#define LARGEUR 9

// Structure pour le déplacement
typedef struct {
    int x;
    int y;
} Curseur;

void nettoyerEcran();
void remplirPlateau(char plateau[HAUTEUR][LARGEUR]);
void afficherPlateau(char plateau[HAUTEUR][LARGEUR]);
void afficherInfos(int niveau, int vies, int coups, int temps);
int estDeMemeCouleur(char a, char b);
int detecterAlignement(char plateau[HAUTEUR][LARGEUR]);
void supprimerAlignements3(char plateau[HAUTEUR][LARGEUR], int collectes[5]);
void appliquerGravite(char plateau[HAUTEUR][LARGEUR]);

void echangerSymboles(char plateau[HAUTEUR][LARGEUR], Curseur c1, Curseur c2);

#endif