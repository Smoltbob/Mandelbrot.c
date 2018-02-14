/* Tests unitaires du module couleur.c */
#include "couleur.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    struct couleur couleur;
    couleur.R = 247;
    couleur.G = 136;
    couleur.B = 170;

    uint32_t entier = 0x00F788AA;
    struct couleur res;

    printf("Couleur -> Entier: 0x%08x\n", couleurtoint32(couleur));
    res = int32tocouleur(entier);
    printf("Entier -> Couleur: R:%i, G:%i, B:%i\n", res.R, res.G, res.B);
    return 0;
}
