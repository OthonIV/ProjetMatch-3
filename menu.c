#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "affichage_console.h"
#include "menu.h"
#include "remplissage.h"

int afficherMenu() {
    int choix = 0;

    nettoyerEcran();

    set_color(CYAN, BLACK);
    printf("\n");
    printf("\t\tECE HEROES\n\n");

    set_color(WHITE, BLACK);
    printf("   1. Lire les regles\n");
    printf("   2. Commencer une nouvelle partie\n");
    printf("   3. Reprendre la partie\n");
    printf("   4. Quitter\n\n");

    printf("   Votre choix : ");

    // lecture sécurisée (évite les bugs scanf)
    choix = getch() - '0';
    printf("%d\n", choix);

    return choix;
}

void afficherRegles() {
    nettoyerEcran();

    set_color(YELLOW, BLACK);
    printf("\n\tREGLES DU JEU\n\n");

    set_color(WHITE, BLACK);
    printf("   1. Alignez 3 symboles identiques.\n");
    printf("   2. Gagnez avant la fin du temps.\n");
    printf("   3. Attention aux vies limitees !\n");
    printf("   4. Les carres de 4 donnent des bonus.\n");
    printf("   5. Amusez-vous !\n\n");

    set_color(LIGHTGRAY, BLACK);
    printf("   Appuyez sur une touche pour revenir au menu...");
    getch();
}