#ifndef GESTION_FICHIER_H
#define GESTION_FICHIER_H

typedef struct {
    int niveauActuel;
    int viesRestantes;
} Progression;

void sauvegarderPartie(Progression p);
int chargerPartie(Progression *p);

#endif