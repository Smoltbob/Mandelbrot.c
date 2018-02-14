/* Module de test du parser */
#include "parser.h"
#include "fractale.h"

int main(int argc, char **argv)
{
    struct parametres param = parseparam(fopen(argv[1], "r"));
}
