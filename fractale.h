#ifndef FRACTALE_H
#define FRACTALE_H

#include <stdint.h>
#include <complex.h>
#include "ppm.h"

struct point
{
    double x;
    double y;
};

struct parametres
{
    uint16_t resol_x;
    uint16_t resol_y;
    double centre_x;
    double centre_y;
    double span_x;
    uint16_t nb_itermax;
};

struct ctrlpoint
{
    float quot;
    uint8_t R;
    uint8_t G;
    uint8_t B;
};

uint32_t *biomorph(struct ppm ppm, struct parametres param, struct ctrlpoint *palette, uint8_t taille);

uint32_t *buddhabrot(struct ppm ppm, struct parametres param);

uint32_t *general(struct ppm ppm, struct parametres param, struct ctrlpoint *palette, uint8_t taille);
#endif
