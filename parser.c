/* Module de lecture du fichier config */
#include "fractale.h"
#include <string.h>
#include <stdlib.h>

/* Ouvre un fichier et retourne les parametres */
struct parametres parseparam(FILE *fic)
{
    /* Lecture du fichier de config. Pour fiabiliser la lecture, mieux 
     * vaut utiliser fgets */
    struct parametres res;
    fscanf(fic, "%hu\n%hu\n%lf\n%lf\n%lf\n%hu", &res.resol_x, &res.resol_y,
            &res.centre_x, &res.centre_y, &res.span_x, &res.nb_itermax);
    return res;
}

uint8_t countlines(FILE *fic)
{
    uint8_t taille = 0;
    char tmp = 0;
    while (!feof(fic))
    {
        tmp = fgetc(fic);
        if (tmp == '\n')
        {
            taille++;
        }
    }
    rewind(fic);
    return taille;
}

struct ctrlpoint *parsepalette(char *filename)
{
    float quot=0.0;
    uint8_t R=0;
    uint8_t G=0;
    uint8_t B=0; 
    char string[20];
    FILE *fic = fopen(filename, "r");
    uint8_t taille = countlines(fic);
    struct ctrlpoint *palette = malloc(taille * sizeof(struct ctrlpoint));
    uint8_t i = 0;
    while (fgets(string, 20, fic))
    {
        sscanf(string, "%f %hhu %hhu %hhu\n", &quot, &R, &G, &B);
        //printf("%f %hhu %hhu %hhu\n", quot, R, G, B);
        palette[i].quot = quot;
        palette[i].R = R;
        palette[i].G = G;
        palette[i].B = B;
        i++; 
    }
    fclose(fic);
    return palette;
}
