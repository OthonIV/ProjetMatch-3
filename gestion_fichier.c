#include <stdio.h>
#include <stdlib.h>
#include "gestion_fichier.h"

// Sauvegarde les données dans un fichier texte
void sauvegarderPartie(Progression p) {
    FILE* fichier = fopen("sauvegarde.txt", "w");
    if (fichier != NULL) {
        fprintf(fichier, "%d %d", p.niveauActuel, p.viesRestantes);
        fclose(fichier);
        printf("Partie sauvegardee avec succes !\n");
    } else {
        printf("Erreur critique lors de la sauvegarde.\n");
    }
}

// Charge les données. Retourne 1 si réussi, 0 sinon.
int chargerPartie(Progression *p) {
    FILE* fichier = fopen("sauvegarde.txt", "r");
    if (fichier != NULL) {
        // On essaye de lire 2 entiers
        if (fscanf(fichier, "%d %d", &p->niveauActuel, &p->viesRestantes) == 2) {
            fclose(fichier);
            return 1; // Succès
        }
        fclose(fichier);
    }
    return 0; // Échec (fichier inexistant ou illisible)
}