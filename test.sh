#!/bin/bash

# Fonctions de génération d'équations
# ex : cpow(z, 1) + 1 + I
# forme : cpow(z, i) + j + k
eq_cpow(){
    declare -n EQ=""
    for i in `seq -3 3`;
    do
        for j in `seq -3 3`;
        do
            for k in `seq -3 3`;
            do
                EQ+=( "cpow(z, $i) + $j + $k" )
            done
        done
    done
}

#eq_cpow
#for i in "${EQ[@]}";
#do
#    echo "$i"
#done

TEST=$(cat favs/19_bioauto.ppm | sed -n 's/.*\(# .*\)/\1/p')
echo "$TEST"
