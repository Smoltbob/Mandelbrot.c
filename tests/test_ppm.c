/* Module de test du module ppm.c */
#include <stdio.h>
#include <stdint.h>
#include "ppm.h"

/* Crée un carré rouge */
void rouge(FILE *fic, struct ppm ppm)
{
    /* Création du tableau de pixels rouges */
    uint32_t taille = ppm.largeur * ppm.hauteur;
    uint32_t entiersrouges[taille];
    for (uint32_t i = 0; i < taille; i++)
    {
        entiersrouges[i] = 0x00FF0000;
    }
    cree_ppm(fic, ppm, entiersrouges);
}

/* Crée un carré noir avec diagonale blanche */
void diag(FILE *fic, struct ppm ppm)
{
    uint32_t taille = ppm.largeur * ppm.hauteur;
    uint32_t entiersdiag[taille];
    for (uint32_t i = 0; i < ppm.hauteur; i++)
    {
        for(uint32_t j = 0; j < ppm.largeur; j++)
        {
            /* Faire une affectation conditionnelle */
            entiersdiag[i*ppm.hauteur+j] = (i == j) ? 0x00FFFFFF : 0x00000000;
        }
    }
    cree_ppm(fic, ppm, entiersdiag);
}

int main(void)
{
    struct ppm ppm;
    ppm.largeur = 2;
    ppm.hauteur = 2;
    ppm.couleurmax = 255;
    
    uint32_t entiers[] = {0x00F788AA, 0x001200C7, 0x00AB4578, 0x00D02101};

    /* Création d'une entete */
    entete_ppm(stdout, ppm);
    /* Création d'un petit fichier */
    cree_ppm(stdout, ppm, entiers);

    /* Création du fichier rouge.ppm */
    FILE *frouge = fopen("rouge.ppm", "a+");
    /* Structure pour fichier rouge */
    struct ppm ppmrouge;
    ppmrouge.largeur = 5;
    ppmrouge.hauteur = 5;
    ppmrouge.couleurmax = 255;

    rouge(frouge, ppmrouge);
    fclose(frouge);

    /* Création du fichier diag.ppm */
    FILE *fdiag = fopen("diag.ppm", "a+");
    /* Structure pour fichier rouge */
    struct ppm ppmdiag;
    ppmdiag.largeur = 5;
    ppmdiag.hauteur = 5;
    ppmdiag.couleurmax = 255;

    diag(fdiag, ppmdiag);
    fclose(fdiag);
    return 0;
}
