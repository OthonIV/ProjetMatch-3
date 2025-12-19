#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> // Pour Sleep
#include "affichage_console.h"
#include "remplissage.h"
#include "menu.h"

int main(void) {
    int choix = 0;
    char plateau[HAUTEUR][LARGEUR];

    int niveau = 1;
    int vies = 3;
    int coups = 0;
    int temps = 60;

    srand(time(NULL));
    hide_cursor();

    do {
        choix = afficherMenu();

        switch (choix) {
            case 1:
                afficherRegles();
                break;

            case 2: // Nouvelle partie
            {
                Curseur sel = {0, 0}; 
                Curseur premier = {-1, -1};
                int enJeu = 1;

                remplirPlateau(plateau);

                while (enJeu) {
                    // --- PHASE 1 : RÉACTION EN CHAÎNE (AUTOMATIQUE) ---
                    int encoreDesAlignements = 1;
                    while (encoreDesAlignements) {
                        // On compte les cases vides
                        int videsAvant = 0;
                        for(int r=0; r<HAUTEUR; r++) 
                            for(int c=0; c<LARGEUR; c++) if(plateau[r][c] == ' ') videsAvant++;

                        supprimerAlignements3(plateau);
                        
                        // On compte après suppression
                        int videsApres = 0;
                        for(int r=0; r<HAUTEUR; r++) 
                            for(int c=0; c<LARGEUR; c++) if(plateau[r][c] == ' ') videsApres++;

                        if (videsApres > videsAvant) {
                            // On affiche les cases vides (l'explosion)
                            gotoxy(0, 0);
                            afficherPlateau(plateau);
                            Sleep(400); 
                            
                            // On fait tomber les pions
                            appliquerGravite(plateau);
                            
                            // On affiche le résultat de la chute
                            gotoxy(0, 0);
                            afficherPlateau(plateau);
                            Sleep(200);
                        } else {
                            // Plus rien ne bouge, on sort de la boucle de cascade
                            encoreDesAlignements = 0; 
                        }
                    }

                    // Affichage du interactif
                    gotoxy(0, 0);
                    afficherPlateau(plateau);
                    
                    // Affichage des infos
                    gotoxy(0, 20); 
                    afficherInfos(niveau, vies, coups, temps);

                    // Affichage du curseur de sélection
                    int posX = sel.x * 4 + 4; 
                    int posY = sel.y * 2 + 1; 

                    gotoxy(posX, posY);

                    // On choisit la couleur selon l'état
                    if (premier.x == sel.x && premier.y == sel.y) {
                        // Si la case est sélectionnée pour un échange
                        set_color(WHITE, BLUE);
                    } else {
                        // Si c'est juste le curseur qui passe
                        set_color(BLACK, LIGHTGRAY);
                    }

                    // On réimprime JUSTE le caractère (sans crochets pour ne rien décaler)
                    printf("%c", plateau[sel.y][sel.x]);

                    // On remet les couleurs normales immédiatement
                    set_color(WHITE, BLACK);

                    // Touches
                    if (kbhit()) {
                        char action = getch();
                        
                        if (action == 'z' || action == 'Z') { if (sel.y > 0) sel.y--; }
                        else if (action == 's' || action == 'S') { if (sel.y < HAUTEUR - 1) sel.y++; }
                        else if (action == 'q' || action == 'Q') { if (sel.x > 0) sel.x--; }
                        else if (action == 'd' || action == 'D') { if (sel.x < LARGEUR - 1) sel.x++; }
                        
                        else if (action == ' ') { // Sélection / Échange
                            if (premier.x == -1) {
                                premier = sel;
                            } else {
                                // Vérifier si les cases sont adjacentes
                                if (abs(premier.x - sel.x) + abs(premier.y - sel.y) == 1) {
                                    // 1. On tente l'échange
                                    echangerSymboles(plateau, premier, sel);
                                    
                                    // 2. On vérifie si cela a créé un alignement
                                    if (detecterAlignement(plateau)) {
                                        // Succès !
                                        coups++;
                                    } else {
                                        // Échec : Pas d'alignement, on affiche brièvement et on annule
                                        gotoxy(0, 0);
                                        afficherPlateau(plateau); 
                                        Sleep(200); // Laisse le temps au joueur de voir l'échange raté
                                        
                                        echangerSymboles(plateau, premier, sel); // On remet en place
                                    }
                                }
                                premier.x = -1; // Désélectionner dans tous les cas
                            }
                        }
                        else if (action == 'o' || action == 'O') {
                            enJeu = 0;
                        }
                    }
                    Sleep(10); // Petit repos CPU
                }
            }
            break;

            case 3: // Reprendre
                // Logique pour charger ou revenir au plateau existant
                break;

            case 4: // Quitter
                nettoyerEcran();
                printf("\n Au revoir !\n");
                break;
        }

    } while (choix != 4);

    show_cursor();
    return 0;
}