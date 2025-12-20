#include <stdio.h>
#include <stdlib.h>
#include <conio.h> // Pour getch()
#include "affichage_console.h"
#include "menu.h"
#include "remplissage.h"

int afficherMenu() {
    int choix = 0;

    nettoyerEcran();

    set_color(CYAN, BLACK);
    printf("\n");
    printf("\t\tECE HEROES\n\n");

    set_color(LIGHTCYAN, BLACK);
    printf("   1. Lire les regles\n");
    set_color(LIGHTGREEN, BLACK);
    printf("   2. Commencer une nouvelle partie\n");
    set_color(LIGHTRED, BLACK);
    printf("   3. Reprendre la sauvegarde\n");
    set_color(MAGENTA, BLACK);
    printf("   4. Quitter\n\n");
    set_color(WHITE, BLACK);
    printf("   Votre choix : ");

    // Lecture sécurisée du chiffre
    choix = getch() - '0';
    printf("%d\n", choix);

    return choix;
}

void afficherRegles() {
    nettoyerEcran();

    set_color(YELLOW, BLACK);
    printf("\n\t--- REGLES DU JEU ---\n\n");

    // Section 1 : Contrôles (Ajoutée)
    set_color(MAGENTA, BLACK);
    printf(" 1. COMMANDES :\n");
    printf("    [ Z Q S D ] : Se deplacer dans le plateau\n");
    printf("    [ ESPACE ]  : Selectionner / Echanger un symbole\n");
    printf("    [ O ]       : Quitter le niveau en cours\n\n");

    // Section 2 : Les bases
    set_color(CYAN, BLACK);
    printf(" 2. BUT DU JEU :\n");
    printf("    Alignez 3 symboles identiques (ou plus) pour remplir le contrat.\n");
    printf("    Attention au temps et au nombre de coups limites !\n\n");

    // Section 3 : Les formes spéciales
    set_color(LIGHTCYAN, BLACK);
    printf(" 3. FORMES SPECIALES :\n");
    printf("    [ 4 Alignes ]       -> Cree un BONUS (= ou H) selon le sens.\n");
    printf("    [ 6 Alignes ]       -> Detruit tous les items de cette couleur.\n");
    printf("    [ Croix 3x3 ]       -> Detruit tous les items de cette couleur.\n");
    printf("    [ Carre de 4x4 ]    -> Explosion de la zone 4x4.\n\n");

    // Section 4 : Les Bonus
    set_color(LIGHTGREEN, BLACK);
    printf(" 4. ITEMS BONUS :\n");
    printf("    Symbole '=' : Detruit toute sa ligne (si aligne).\n");
    printf("    Symbole 'H' : Detruit toute sa colonne (si aligne).\n\n");

    // Section 5 : Vie et Sauvegarde
    set_color(LIGHTRED, BLACK);
    printf(" 5. PROGRESSION :\n");
    printf("    Echec du niveau = Perte d'une vie.\n");
    printf("    La progression est sauvegardee entre chaque niveau.\n\n");

    set_color(WHITE, BLACK);
    printf(" Appuyez sur une touche pour revenir au menu...");
    getch();
}