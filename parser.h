#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "fractale.h"

struct parametres parseparam(FILE*);
struct ctrlpoint *parsepalette(char *fic);
uint8_t countlines(FILE *fic);

#endif
