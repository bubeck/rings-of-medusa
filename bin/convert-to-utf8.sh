#!/bin/bash

for f in "$@"; do
    if [ -f "$f" ]; then
	iconv -f cp437 -t UTF-8 "$f" -o /tmp/atari.txt
	sed -i 's/₧/ß/g' /tmp/atari.txt
	sed -i 's/╜/\(C\)/g' /tmp/atari.txt
	sed -i 's/\x03/->/g' /tmp/atari.txt
	sed -i 's/\x04/<-/g' /tmp/atari.txt
	sed -i 's/\x08/./g' /tmp/atari.txt
	sed -i 's/\r$//' /tmp/atari.txt
	sed -i 's/\x1a$//' /tmp/atari.txt
	touch -r "$f" /tmp/atari.txt
	cp /tmp/atari.txt "$f"
	touch -r /tmp/atari.txt "$f"
    fi
done




