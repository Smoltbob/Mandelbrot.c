/* Ce module gère la création d'images ppm */
#include <stdio.h>
#include <stdint.h>
#include "ppm.h"
#include "couleur.h"

/* Affiche l'en-tête du fichier ppm, format P6 (binaire) en utilisan en
 * entrée une structure qui décrit les caractérisitiques du fichier */
void entete_ppm(FILE *fic, struct ppm ppm)
{
    fprintf(fic, "P6\n");
    fprintf(fic, "%hu %hu\n", ppm.largeur, ppm.hauteur);
    fprintf(fic, "%i\n", ppm.couleurmax);
}

/* Crée un fichier ppm à partir d'une structure décrivant les caractérisitiques
 * et d'un tableau de pixels RGB décrits par un des entiers 32 bits */
void cree_ppm(FILE *fic, struct ppm ppm, uint32_t *image)
{
    /* Copie locale du pointeur sur le tableau */
    uint32_t *i32pixel = image;
    /* Génération de l'en-tête apropriée */
    entete_ppm(fic, ppm);
    /* On recopie chaque pixel du tableau */
    for(uint32_t j = 0; j < ppm.hauteur * ppm.largeur; j++)
    {
        /* Extraction des coordonnées RGB à partir de l'entier */
        struct couleur pixel = int32tocouleur(*i32pixel);
        /* On place les valeurs dans un tableau et on les écrit en même
         * temps */
        /* Combiner plus de pixels en même temps dans un grand tableau pour
         * être plus rapide ? */
        uint8_t couleurs[] = {pixel.R, pixel.G, pixel.B};
        fwrite(couleurs, 1, 3, fic);
        i32pixel++;
    }
} 
