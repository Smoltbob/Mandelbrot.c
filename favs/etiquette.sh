#!/bin/bash
for file in *.ppm;
do
    tmp=$(grep -a -o '# .*' "$file")
    echo "$file : $tmp"
    convert -comment "$tmp" "$file" "${file%.ppm}.jpg"
done
