#!/bin/bash
# Ce script modifie  la ligne qui correspond à l'équation utilisée pour les
# biomorphes dans la fonction equation de fractale.c

# Fonctions de génération d'équations
# ex : cpow(z, 1) + 1 + I
# forme : cpow(z, i) + j + k
eq_cpow(){
    declare -n CPOW=""
    for i in `seq -10 10`;
    do
        for j in `seq -10 10`;
        do
            for k in `seq -10 10`;
            do
                CPOW+=( "cpow(z, $i) + $j + $k * I" )
            done
        done
    done
}

eq_sin(){
    declare -n CPOW=""
    for j in `seq -5 5`;
    do
        for k in `seq -5 5`;
        do
            CPOW+=( "sin(z) + $j + $k * I" )
        done
    done
}
CPT=0
DOSSIER="biomorphes"
mkdir -p $DOSSIER

eq_cpow
for EQUATION in "${CPOW[@]}";
do
    # Etape 1 : trouver la ligne qu suit le mot " @Biomorphe et la modifier
    # n = modifier apres ligne. -i = directement modifier le fichier
    sed -i "/@Biomorphe/{n;s/.*/return $EQUATION;/}" fractale.c

    # Etape 2 : compiler le fichier
    make test_fractale

    # Etape 3 : générer la nouvelle image dans un dossier
    ./test_fractale configs/bioconfig palettes/biobase ""$DOSSIER"/"$CPT"_bioauto.ppm"

    # Etape 4 : ecrire l'equation en commentaire de l'image
    echo "# $EQUATION" >> ""$DOSSIER"/"$CPT"_bioauto.ppm"
    CPT=$(($CPT + 1))

    # Etape 5 : convertir en JPEG
done
