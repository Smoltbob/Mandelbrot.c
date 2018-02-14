/* Module de test des fractales */
#include "fractale.h"
#include "ppm.h"
#include "parser.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* Idée: pour des ultra hautes résolutions et pour améliorer la vitesse
 * diviser l'image finale en sous images qui peuvent être calculées en 
 * parallèle puis recoller les fichier ppm ensemble via des manipulations
 * de texte */

/* Test de l'appel à une fonction de fractales */
void test_mandel(FILE *fic, struct ppm ppm, struct parametres param, struct ctrlpoint *palette, uint8_t taille)
{
    /* Calcul du tableau de pixels */
    //uint32_t *image = general(ppm, param, palette, taille);
    uint32_t *image = biomorph(ppm, param, palette, taille);
    //uint32_t *image = buddhabrot(ppm, param);
    cree_ppm(fic, ppm, image);
    free(image);
    /* Mieux : combiner calcul du tableau et création de l'image ? */
}

int main(int argc, char **argv)
{
    /* Lecture du fichier config indiqué en argument */
    struct parametres param = parseparam(fopen(argv[1], "r"));
    /* Lecture de la palette de couleurs */
    FILE *tmp = fopen(argv[2], "r");
    uint8_t taille = countlines(tmp);
    fclose(tmp);
    struct ctrlpoint *palette = parsepalette(argv[2]);
    /* Création de la strucuture qui décrit l'image */
    struct ppm ppmmandel;
    ppmmandel.largeur = param.resol_x;
    ppmmandel.hauteur = param.resol_y;
    ppmmandel.couleurmax = 255;

    /* Création de la fractale */
    FILE *fmandel = fopen(argv[3], "w");
    test_mandel(fmandel, ppmmandel, param, palette, taille);
    free(palette);
    fclose(fmandel);
}
