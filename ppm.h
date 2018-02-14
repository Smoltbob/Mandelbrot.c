#ifndef PPM_H
#define PPM_H

#include <stdint.h>
#include <stdio.h>

struct ppm
{
    uint16_t hauteur;
    uint16_t largeur;
    uint8_t couleurmax;
};

/* Affiche l'en-tête en ascii */
void entete_ppm(FILE*, struct ppm);
/* Généère le code BINAIRE  du fichier ppm */
void cree_ppm(FILE*, struct ppm, uint32_t*);

#endif
