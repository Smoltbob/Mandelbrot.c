/* Ce module définit les fonctions pour la création de fractales */
#include <stdlib.h>
#include <tgmath.h>
#include <complex.h>
#include <stdbool.h>
#include "fractale.h"
#include "ppm.h"
#include "couleur.h"

/* En entrée : un point dans le le plan de l'image [0, resx]x[0, resy]
 * En sortie : ces coordonnées dans le plan complexe [-2.2, 0.8]x[1.5, 1.5]
 * par exemple pour mandelbrot.
 * Notation : [x1, x2]x[y1, y2]
 */
struct point map_point(struct point point, struct parametres param)
{
    /* Lecture des parametres 
     * Ce sont les paramètres que l'on écrit dans le fichier config
     * de la fractale. 
     */
    int16_t etendue_entreex = param.resol_x;
    int16_t etendue_entreey = param.resol_y;
    double span = param.span_x;
    double spanx = param.span_x;

    double min_sortiex;
    /* Si on a un rectangle, on modifie les ratios dans l'axe des abscisses
     * et on centre l'image
     * */
    if(param.resol_x != param.resol_y)
    {
       double ratio = ((double)param.resol_x/param.resol_y);
       spanx *= ratio;
       min_sortiex = ((double)param.centre_x - spanx/2);
    }
    else
    {
       min_sortiex = param.centre_x - span/2;
    }
    double min_sortiey = param.centre_y - span/2;

    /* Définition du point mappé en sortie */
    struct point res;
    /* Calcul du x */
    res.x = (spanx/etendue_entreex)*point.x + min_sortiex;
    /* Calcul du y */
    res.y = (span/etendue_entreey)*point.y + min_sortiey;

    return res;
}

/* Pour un intervalle [qdep, qfin] on associe une intervalle de valeurs
 * [valdep, valfin]. Pour une valeur q incluse dans [qdep, qfin] on 
 * cherche sa valeur correspondante dans [valdep, valfin]
 */
uint8_t colormap(float q, float qdep, float qfin, uint8_t valdep, uint8_t valfin)
{
    uint8_t res;
    res = valdep + (q - qdep)*(valfin - valdep)/(qfin - qdep);
    return res;
}

/* Retourne la couleur correspondant à un quotient d'échappement donné. */
uint32_t color_palette(float quot, struct ctrlpoint *palette, uint8_t taille)
{
    /* Nous avons besoin de trouver dans quel point de contrôle ce trouve
     * le quotient passé en paramètre */
    uint8_t index = 0;
    struct couleur col;
    bool trouve = false;
    /* Si il n'y a qu'un point de contrôle la même couleur est appliquée
     * partout */
    if (taille > 1)
    {
        /* Recherche de l'index correct */
        uint8_t i = 0;
        while (trouve == false)
        {
            if (quot < palette[i + 1].quot)
            {
                index = i;
                trouve = true;
            }
            i++;
        }
    }
    /* Calcul de la couleur en sorite en fonction de notre position dans la
     * palette et du quotient
     */
    col.R = colormap(quot, palette[index].quot, palette[index+trouve].quot,
            palette[index].R, palette[index+trouve].R);

    col.G = colormap(quot, palette[index].quot, palette[index+trouve].quot,
            palette[index].G, palette[index+trouve].G);

    col.B = colormap(quot, palette[index].quot, palette[index+trouve].quot,
            palette[index].B, palette[index+trouve].B);

    uint32_t res = couleurtoint32(col);

    return res;
}

float smoothcolor(float complex z, uint32_t iter, uint32_t max_iter)
{
    float modulus = fabs(z);
    float mu = iter - (log (log (modulus)))/ log(2.0);
    return mu/max_iter;
}



 /* TODO remplacer le struct par un complexe (remplacer TOUS les structs ?
    Choix : 0 = Mandelbrot
            1 = Tricorn
            2 = Burning Ship
            3 = Julia
 * */
float complex equation(float complex z, struct point point, uint8_t choix)
{
    static double complex h[] = { -1.0 + I, -0.2 + I, I,                   // 0, 1, 2
        -0.8 + 0.8*I, -0.4 + 0.8*I, -0.2 + 0.8*I, 0.8*I,            // 3,4,5,6
        -0.6 + 0.6*I, -0.4 + 0.6*I, 0.6*I,                          // 7,8,9
        0.19 + 0.4*I, 0.39 + 0.4*I, -0.8 + 0.4*I, -0.6 + 0.4*I,     // 10,11,12,13
        0.59 + 0.4 * I, -1.0 + 0.4 * I,  -0.8 + 0.4*I, 0.19 + 0.4*I,// 14, 15, 16, 17
        0.39 + 0.4*I,                                               // 18
        -1.0 + 0.2*I, 0.8 + 0.2*I, 0.39 + 0.2*I,                    // 19,20,21
        -1.0,-0.8, -0.6, 0.39, 0.59};                               // 22,23,24,25,26
    switch (choix)
    {
        case 0:
            // Mandelbrot
            return cpow(z, 2) + point.x + I*point.y;
            break;
        case 1:
            // Tricorn
            return pow(conj(z), 2) + point.x + I*point.y;
            break;
        case 2:
            // Burning ship
            return pow(fabs(creal(z)) + I*fabs(cimag(z)), 2) + point.x + I*point.y;
            break;
        case 3:
            // Julia
            return pow(z, 2) + h[12];
            break;
        case 4:
            // @Biomorphes
return cpow(z, 10) + 10 + 10;
            break;
        default:
            return 0;
            break;
    }
}

bool condition(float complex z, uint8_t choix)
{
    switch (choix)
    {
        // Classique
        case 0:
            return fabs(z) <= 2;
            break;
        // Biomorphe
        case 1:
            return (cabs(z) <= 10);
            break; 
        default:
            return false;
            break;
    }

}

// Cette fonction calcule la fractale dans son ensemble
float complex calcfract(struct point ptcour, uint32_t max_iter, uint32_t *iter, uint8_t choix)
{
    float complex z;
    uint8_t choixcond = 0;

    if(choix == 3)
        z = ptcour.x + I*ptcour.y;
    else
        z = 0;

    while (condition(z, choixcond) && *iter < max_iter)
    {
        z = equation(z, ptcour, choix);
        (*iter)++;
    }
    return z;
}

/* Calcule la coloration de chaque pixel.
 * Choix :  0 = pas de smoothing, valeur absolue
 *          1 = smoothing, valeur absolue
 */
float quotcolor(uint8_t choixcol, uint8_t choixeq, float complex *z, struct point mpoint0, uint32_t max_iter, uint32_t *iter)
{
    float quot = 0;
    /* Smoothing : on itère 3 fois pour supprimer le banding */
    switch (choixcol)
    {
        case 0:
            quot = (float)(*iter)/max_iter - 0.000001;
            break;
        case 1:
            for(int8_t j = 0; j < 3; j++)
            {
                *z = equation(*z, mpoint0, choixeq);
                (*iter)++;
            }
            /* Si on a divergé on colore le point en fonction du rang de
             * divergence. */
            /* Smoothcolor utilise une opération logarithmique. */
            if (*iter - 3 < max_iter)
            {
                quot = smoothcolor(*z, *iter, max_iter);
            }
            (*iter) -= 3;
            break;
    }
    return quot;
}


/* Fonction de génération des fractales Biomorphes
 * Particularités de ces fractales :
 *      - La condition de sortie pour sur la valeur absolue de z (ou de sa partie réelle / imaginaire
 *      - La coloration dépend de la valeur absolue de la partie réelle ou imaginaire
 *      - Il suffit de 10 itérations au max pour le tracer, quel que soit le zoom
 */
uint32_t *biomorph(struct ppm ppm, struct parametres param, struct ctrlpoint *palette, uint8_t taille)
{  
    struct point point0;
    struct point mpoint0;
    uint32_t *ret = malloc(ppm.hauteur*ppm.largeur*sizeof(uint32_t));

    /* On a besoin de deux quotients pour la coloration : 
     * un pour les réels et un pour les imaginaires */
    float quotR = 0.0;
    float quotI = 0.0;

    /* Création d'une palette temporaire pour les imaginaires
     * (la palette fournie en argument est utilisée pour les réels)
     * Ici : tons maron / bronze */
    struct ctrlpoint paletteI[3];
    paletteI[0] = (struct ctrlpoint){0, 250, 250, 250};
    paletteI[1] = (struct ctrlpoint){0.45, 160, 130, 50};
    paletteI[2] = (struct ctrlpoint){1, 40, 40, 40};

    /* On itère sur tous les pixels de l'image désirée */
    for (uint32_t i = 0; i < ppm.largeur * ppm.hauteur; i++)
    {
        /* Convertion du tableau 1D en points 2D */
        point0.y = (i / ppm.largeur);
        point0.x = i % ppm.largeur;
        /* Mappage dans l'étendue de Mandelbrot */
        mpoint0 = map_point(point0, param);
        /* Début du calcul du set */
        /* Définition du point de départ de la fonction récursive */
        double complex z = mpoint0.x + I*mpoint0.y; 
        /* Initialisation des variables d'itération */
        uint32_t iter = 0;
        uint32_t max_iter = param.nb_itermax;
        /* Calcul de la divergence du point */
        while ((cabs(z) < 10) && iter < max_iter)
        {
            // 4 = Biomorphe
            z = equation(z, mpoint0, 4);
            iter++;
        }
        /* Si on est sorti du while avant la fin : on a divergé */
        /* Affectation des quotients pour coloration */
        /* On s'assure que le quotient ne puisse être >= 1 */
        if (fabs(creal(z)) < max_iter)
            quotR = fabs(creal(z))/max_iter;
        if (fabs(cimag(z)) < max_iter)
            quotI = fabs(cimag(z))/max_iter;

        /* Coloration */
        if (fabs(creal(z)) < 10)
        {
            ret[i] = color_palette(quotR, palette, taille);
        }
        else if (fabs(cimag(z)) < 10)
        {
            ret[i] = color_palette(quotI, paletteI, taille);
        }
        /* Couleur de fond pour les points convergents */
        else
        {
            ret[i] = 0x00FEFFFE;
        }
    }
    return ret;
}

uint32_t *buddhabrot(struct ppm ppm, struct parametres param)
{  
    uint32_t *ret = malloc(ppm.hauteur*ppm.largeur*sizeof(uint32_t));

    uint16_t max_iter = 2000;
    uint16_t min_iter = 100;

    /* Tableau pour les nuances de gris */
    uint16_t *gris = malloc(ppm.largeur*ppm.hauteur*sizeof(uint16_t));

    /* Points dont on utilise la trajectoire pour tracer la fractale */
    uint16_t jx;
    uint16_t jy;

    /* Variable de la suite de Mandelbrot */
    float complex z = 0;
    /* Constante de la suite de Mandelbrot */
    float complex c = 0;

    /* Calcul de la fractale */
    /* Plus le nombre d'itérations est grand, plus on essaye de points */
    for (uint32_t i = 0; i < 60000000; i++)
    {
        /* Sélection de deux points aléatoires entre -2 et 2 (?) */
        /* Sinon : rand() % (max_number + 1 - minimum_number) + minimum_number */
        float x = (float)rand()*4.0f/((float)RAND_MAX+1.0f)-2.0f;
        float y = (float)rand()*4.0f/((float)RAND_MAX+1.0f)-2.0f;
        /* Initialisation de la suite */
        z = 0;
        c = x + I*y;
        /* On regarde si le point appartient à l'ensemble M de Mandelbrot */
        uint32_t iter = 0;
        while (fabs(z) < 2 && iter < max_iter)
        {
            z = z*z + c;
            iter++;
        }
        /* Si le point n'appartient pas à l'ensemble alors un le garde.
         * Cela correspondrait à un pixel coloré sur la fractale de Mandelbrot 
         * classique.
         * Pour savoir si le point n'appartient pas à l'ensemble on regarde 
         * la variable iter */
        /* Pour chaque point que l'on garde, on les fait passer dans la suite
         * de Mandelbrot. A chaque itération on regarde la position du point.
         * A chacune de ses positions est associé un pixel. A chaque fois que
         * l'on passe sur un pixel, on veut le rendre plus clair.
         * Pour cela on utilise le tableau gris[] que l'on incrémente. */
        /* A la fin de l'itération le pixel a convergé vers sa valeur finale
         * dans la suite (?)*/
        if (iter < max_iter && iter > min_iter)
        {
            z = c;
            for (uint32_t j = 0; j < iter-1; j++)
            {
                /* Détermination des coordonnées du pixel
                 * (on passe de l'espace de mandelbrot vers l'espace de notre
                 * image) */
                jx = (crealf(z)+2.0f)/(4.0f/ppm.largeur);
                jy = (cimagf(z)+2.0f)/(4.0f/ppm.hauteur);
                /* Incrémentation du tableau */
                gris[jx*ppm.largeur + jy]++;
                z = z*z + c;
            }
        }
    }
    /* Affectation des couleurs (nuances de gris) */
    uint16_t max = 0;
    for (uint32_t i = 0; i < ppm.hauteur*ppm.largeur; i++)
    {
        /* Recherche de la valeur max == la valeur qui correspondra à un
         * pixel blanc */
        if (gris[i] > max)
            max = gris[i];
    }
    printf("%u\n", max);
    /* Affectation des couleurs. Ici : tout gris */
    for (uint32_t i = 0; i < ppm.hauteur*ppm.largeur; i++)
    {
        ret[i] = 0;
        ret[i] |= gris[i]*255/max;
        ret[i] |= gris[i]*255/max << 8;
        ret[i] |= gris[i]*255/max << 16;
    }
    free(gris);
    return ret;
}

/* A moduler :
 * TODO : méthode de coloration
 * TODO : complètement gérer Julia
 * TODO : trouver un moyen de bien colorier
 * TODO : méthode de rendu
 * TODO : mieux gérer les couleurs
 * TODO : parallèliser
 * */
uint32_t *general(struct ppm ppm, struct parametres param, struct ctrlpoint *palette, uint8_t taille)
{  
    struct point point0;
    struct point mpoint0;
    uint32_t *ret = malloc(ppm.hauteur*ppm.largeur*sizeof(uint32_t));

    float quot = 0;
    float complex z = 0;
    uint8_t choixeq = 0;
    uint8_t choixcol = 1;
    uint8_t choixcond = 0;

    for (uint32_t i = 0; i < ppm.largeur * ppm.hauteur; i++)
    {
        /* Convertion du tableau 1D en points 2D */
        point0.y = (i / ppm.largeur);
        point0.x = i % ppm.largeur;
        /* Mappage dans l'étendue de Mandelbrot */
        mpoint0 = map_point(point0, param);
        /* Début du calcul du set */
        uint32_t iter = 0;
        uint32_t max_iter = param.nb_itermax;
        /* Calcul des points de la fractale */
        z = calcfract(mpoint0, max_iter, &iter, choixeq);
        /* Calcul du coefficient de coloration */
        quot = quotcolor(choixcol, choixeq, &z, mpoint0, max_iter, &iter);
        /* Génération de l'image */
        ret[i] = (! condition(z, choixcond)) ? color_palette(quot, palette, taille) : 0x00000000;
    }
    return ret;
}
