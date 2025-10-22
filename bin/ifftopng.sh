#!/bin/bash

output1="$(dirname "$1")/$(basename $1 .iff).neo"
output2="$(dirname "$1")/$(basename $1 .iff).png"
echo "$1 -> $output"

python ~/buero/software/atari/fix-atari-iff.py "$1" -o /tmp/atari.iff -v >& /tmp/output.log

if grep -q RAST /tmp/output.log ; then
    echo "RAST found in $1"
    exit 1
fi

ilbmtoppm /tmp/atari.iff > /tmp/atari.ppm
#ppmtoneo /tmp/atari.ppm > "$output1"
pnmtopng /tmp/atari.ppm > "$output2"

