#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
            case 1: // Règles
                afficherRegles();
                break;

            case 2: // Nouvelle partie
            {//Ouverture variables locales
                Curseur sel = {0, 0}; 
                Curseur premier = {-1, -1};
                int enJeu = 1;

                remplirPlateau(plateau);

                while (enJeu) {
                    gotoxy(0, 0); 
                    afficherPlateau(plateau);
                    supprimerAlignements3(plateau);
                    appliquerGravite(plateau);
                    afficherPlateau(plateau);

                    // Positionnement FIXE des informations sous le plateau
                    gotoxy(0, 20); 
                    afficherInfos(niveau, vies, coups, temps);
                    
                    if(premier.x != -1) {
                        printf(" Selection active : [%d,%d] ", premier.x, premier.y);
                    } else {
                        printf("                                     "); // Efface le texte précédent
                    }

                    int consoleX = sel.x * 4 + 4;
                    int consoleY = sel.y * 2 + 1;
                    
                    gotoxy(consoleX, consoleY);
                    set_color(BLACK, WHITE);
                    printf("%c", plateau[sel.y][sel.x]);
                    set_color(WHITE, BLACK);

                    char action = getch();
                    
                    // Déplacement
                    if (action == 'z' && sel.y > 0) sel.y--;
                    if (action == 's' && sel.y < HAUTEUR - 1) sel.y++;
                    if (action == 'q' && sel.x > 0) sel.x--;
                    if (action == 'd' && sel.x < LARGEUR - 1) sel.x++;
                    
                    // Sélection et Échange
                    if (action == ' ') {
                        if (premier.x == -1) {
                            premier = sel;
                        } else {
                            // Calcul de distance pour l'adjacence
                            if (abs(premier.x - sel.x) + abs(premier.y - sel.y) == 1) {
                                echangerSymboles(plateau, premier, sel);
                                coups++;
                                supprimerAlignements3(plateau);
                                appliquerGravite(plateau);
                                afficherPlateau(plateau);
                            }
                            premier.x = -1; 
                        }
                    }

                    if (action == 'o' || action == 'O') enJeu = 0;
                }
            }// Fermeture variables locales
            break;

            case 3: // Reprendre partie
                nettoyerEcran();
                afficherInfos(niveau, vies, coups, temps);
                afficherPlateau(plateau);

                printf("\n Appuyez sur une touche pour revenir au menu...");
                getch();
                break;

            case 4: // Quitter
                nettoyerEcran();
                set_color(LIGHTGRAY, BLACK);
                printf("\n Au revoir !\n");
                break;

            default:
                // Choix invalide
                break;
        }

    } while (choix != 4);

    show_cursor();
    set_color(WHITE, BLACK);
    return 0;
}
