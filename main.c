#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> // Pour Sleep
#include "affichage_console.h"
#include "remplissage.h"
#include "menu.h"
#include "gestion_fichier.h"


// Renommée pour éviter le conflit avec "Progression" de gestion_fichier.h
typedef struct {
    int objectifs[5]; 
    int totalNiveau;
} ContratNiveau;

extern char symboles[5]; 

void initialiserObjectifs(ContratNiveau *c, int niveau) {
    int totalCible = (niveau == 1) ? 60 : (niveau == 2) ? 100 : 150;
    c->totalNiveau = totalCible;
    for(int i=0; i<5; i++) c->objectifs[i] = totalCible / 5;
}

int main(void) {
    int choix = 0;
    char plateau[HAUTEUR][LARGEUR];
    
    // Pour la sauvegarde
    Progression maPartie = {1, 3}; 

    srand(time(NULL));
    hide_cursor();

    do {
        choix = afficherMenu();
        int lancerJeu = 0; // Flag pour savoir si on lance la boucle de jeu

        switch (choix) {
            case 1:
                afficherRegles();
                break;

            case 2: // Nouvelle partie
                maPartie.niveauActuel = 1;
                maPartie.viesRestantes = 3;
                lancerJeu = 1;
                break;

            case 3: // Charger sauvegarde
                if (chargerPartie(&maPartie)) {
                    printf("\n Sauvegarde chargee : Niveau %d, %d Vies.\n", maPartie.niveauActuel, maPartie.viesRestantes);
                    Sleep(1500);
                    lancerJeu = 1;
                } else {
                    set_color(LIGHTRED, BLACK);
                    printf("\n Aucune sauvegarde trouvee !\n");
                    Sleep(1500);
                }
                break;

            case 4: // Quitter
                nettoyerEcran();
                printf("\n Au revoir !\n");
                break;
        }

        // Boucle principale du jeu. (mise ici et non au milieu du code car assez indigeste)
        if (lancerJeu) {
            int victoireTotale = 0;

            while (maPartie.niveauActuel <= 3 && maPartie.viesRestantes > 0) {
                ContratNiveau contrat;
                initialiserObjectifs(&contrat, maPartie.niveauActuel);

                Curseur sel = {0, 0}; 
                Curseur premier = {-1, -1};
                int enNiveau = 1;
                
                // Gestion des coups limités selon le niveau
                int coupsRestants = (maPartie.niveauActuel == 1) ? 20 : (maPartie.niveauActuel == 2) ? 15 : 12;
                
                time_t debutTemps = time(NULL);
                int tempsLimite = 90; 

                remplirPlateau(plateau);

                while (enNiveau) {
                    time_t actuel = time(NULL);
                    int tempsRestant = tempsLimite - (int)difftime(actuel, debutTemps);

                    // Conditions de défaite
                    if (tempsRestant <= 0 || coupsRestants < 0) { 
                        nettoyerEcran();
                        set_color(LIGHTRED, BLACK);
                        if (tempsRestant <= 0) printf("\n TEMPS ECOULE ! \n");
                        else printf("\n PLUS DE COUPS ! \n");
                        
                        maPartie.viesRestantes--;
                        printf(" Vies restantes : %d\n", maPartie.viesRestantes);
                        
                        Sleep(2000);
                        enNiveau = 0; // On sort du niveau pour relancer ou quitter
                        
                        // Si plus de vie Game Over (très bonne BD d'ailleurs)
                        if (maPartie.viesRestantes <= 0) {
                            maPartie.niveauActuel = 4; // Force la sortie de boucle
                        } else {
                            // On propose de save et quitter après une perte de vie
                            nettoyerEcran();
                            printf("Voulez-vous reessayer tout de suite (1) ou Sauvegarder et Quitter (2) ? ");
                            int decision = getch();
                            if (decision == '2') {
                                sauvegarderPartie(maPartie);
                                enNiveau = 0;
                                maPartie.niveauActuel = 100; // Code pour sortir de la boucle de jeu et revenir au menu
                            }
                        }
                        break;
                    }

                    // Gestion des alignements
                    int collectesCeTour[5] = {0};
                    if (detecterAlignement(plateau)) {
                        gotoxy(0, 0);
                        afficherPlateau(plateau);
                        Sleep(400); 

                        supprimerAlignements3(plateau, collectesCeTour, &maPartie.viesRestantes);
                        
                        for(int i=0; i<5; i++) {
                            contrat.objectifs[i] -= collectesCeTour[i];
                            if (contrat.objectifs[i] < 0) contrat.objectifs[i] = 0;
                        }

                        gotoxy(0, 0);
                        afficherPlateau(plateau);
                        Sleep(300);
                        appliquerGravite(plateau);
                    }

                    // Vérification de la victoire
                    int reste = 0;
                    for(int i=0; i<5; i++) reste += contrat.objectifs[i];
                    
                    if (reste <= 0) {
                        nettoyerEcran();
                        set_color(LIGHTGREEN, BLACK);
                        printf("\n BRAVO ! NIVEAU %d REUSSI !\n", maPartie.niveauActuel);
                        Sleep(2000);
                        
                        maPartie.niveauActuel++;
                        enNiveau = 0;

                        // Proposition de sauvegarde entre les niveaux
                        if (maPartie.niveauActuel <= 3) {
                            nettoyerEcran();
                            printf("Niveau suivant (1) ou Sauvegarder et Quitter (2) ? ");
                            int decision = getch();
                            if (decision == '2') {
                                sauvegarderPartie(maPartie);
                                maPartie.niveauActuel = 100; // Sortie vers menu
                            }
                        } else {
                            victoireTotale = 1;
                        }
                        continue;
                    }

                    // Affichage du plateau et des infos
                    gotoxy(0, 0);
                    afficherPlateau(plateau);
                    
                    set_color(CYAN, BLACK);
                    gotoxy(45, 2); printf("OBJECTIFS NIVEAU %d:", maPartie.niveauActuel);
                    for(int i=0; i<5; i++) {
                        gotoxy(45, 4+i);
                        printf("%c restant: %2d  ", symboles[i], contrat.objectifs[i]);
                    }
                    
                    if (coupsRestants <= 5) set_color(LIGHTRED, BLACK);
                    else set_color(YELLOW, BLACK);
                    gotoxy(45, 10); printf("COUPS RESTANTS: %d ", coupsRestants);
                    
                    gotoxy(0, 20); 
                    afficherInfos(maPartie.niveauActuel, maPartie.viesRestantes, coupsRestants, tempsRestant);

                    // Curseur de sélection et échanges
                    int posX = sel.x * 4 + 4; 
                    int posY = sel.y * 2 + 1; 
                    gotoxy(posX, posY);
                    if (premier.x == sel.x && premier.y == sel.y) set_color(WHITE, BLUE);
                    else set_color(BLACK, LIGHTGRAY);
                    printf("%c", plateau[sel.y][sel.x]);
                    set_color(WHITE, BLACK);

                    if (kbhit()) {
                        char action = getch();
                        if (action == 'z' || action == 'Z') { if (sel.y > 0) sel.y--; }
                        else if (action == 's' || action == 'S') { if (sel.y < HAUTEUR - 1) sel.y++; }
                        else if (action == 'q' || action == 'Q') { if (sel.x > 0) sel.x--; }
                        else if (action == 'd' || action == 'D') { if (sel.x < LARGEUR - 1) sel.x++; }
                        else if (action == ' ') {
                            if (premier.x == -1) premier = sel;
                            else {
                                if (abs(premier.x - sel.x) + abs(premier.y - sel.y) == 1) {
                                    echangerSymboles(plateau, premier, sel);
                                    if (detecterAlignement(plateau)) {
                                        coupsRestants--; 
                                    } else {
                                        echangerSymboles(plateau, premier, sel); 
                                    }
                                }
                                premier.x = -1;
                            }
                        }
                        else if (action == 'k') { // Touche pour tester la sauvegarde
                             sauvegarderPartie(maPartie);
                             enNiveau = 0; maPartie.niveauActuel = 100;
                        }
                    }
                    Sleep(10);
                }
            }

            if (victoireTotale) {
                nettoyerEcran();
                set_color(LIGHTMAGENTA, BLACK);
                printf("\n FELICITATIONS ! VOUS AVEZ SAUVE L'ECE ! \n");
                Sleep(3000);
            } else if (maPartie.viesRestantes <= 0) {
                nettoyerEcran();
                set_color(RED, BLACK);
                printf("\n GAME OVER - PLUS DE VIES \n");
                Sleep(3000);
            }
        }

    } while (choix != 4);

    show_cursor();
    return 0;
}