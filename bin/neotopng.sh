#!/bin/bash

output1="$(dirname "$1")/$(basename $1 .neo).png"
echo "$1 -> $output1"

neotoppm "$1" > /tmp/atari.ppm
pnmtopng /tmp/atari.ppm > "$output1"

