#!/bin/sh

for img in *.[Jj][Pp][Gg]
do
    nimg=$(echo $img | sed -e 's/[Jj][Pp][Gg]$/png/')
    echo "convert from $img to $nimg"
    convert "$img" "$nimg"
    rm "$img"
done

