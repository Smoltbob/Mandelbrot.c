/* Ce module définit les méthodes permettant d'accéder aux différentes
 * composantes des pixels de l'image.
 * L'image est représenté par un tableau d'entiers sur 32 bits
 */
#include "couleur.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Retourne les diférentes couleurs codées par l'entier 
 * Structure de l'entier : octet de poids fort 0
 * Puis : R, G, B
 * */
struct couleur int32tocouleur(uint32_t entier)
{
    /* Variable qui contient le résultat */
    struct couleur res;
    /* On utilise des masques : on utilise le ET bit à bit pour  ne 
     * récupérer qu'une portion de l'entier à chaque fois
     * */
    /* Autre méthode pour accéder au quatrimeme octet de poids fort par
     * exemple :
     * res.R = ((uint8_t*) &entier)[3];
     * Attention : dépend de la machine (big endian / little endian
     */
    res.R = (entier >> 4*4) & 0xFF;
    res.G = (entier >> 2*4) & 0xFF;
    res.B = entier & 0xFF;
    return res;
}

/* Retourne un entier à partir des couleurs désirées */
uint32_t couleurtoint32(struct couleur couleur)
{
    /* Variable qui contient le résultat */
    uint32_t res = 0;
    /* On utilise des masques : le OU bit à bit permet d'affecter à res
     * les bits représentés par les champs de res que l'on a décallé de
     * manière appropriée.
     */
    res |= couleur.R << 4*4;
    res |= couleur.G << 2*4;
    res |= couleur.B << 0;
    return res;
}
