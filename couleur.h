#ifndef COULEUR_H
#define COULEUR_H

#include <stdint.h>
#include <stdio.h>

struct couleur
{
    uint8_t R;
    uint8_t G;
    uint8_t B;
};

/* Retourne les diférentes couleurs codées par l'entier 
 * Structure de l'entier : octet de poids fort 0
 * Puis : R, G, B
 * */
struct couleur int32tocouleur(uint32_t);

/* Retourne un entier à partir des couleurs désirées */
uint32_t couleurtoint32(struct couleur);

/* Affiche un triplet de couleurs */
void affiche_triplet32(FILE*, uint32_t);
#endif
