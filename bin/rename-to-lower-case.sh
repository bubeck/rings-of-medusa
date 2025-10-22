#!/bin/bash

find . -depth -name '*[A-Z]*' | while read f; do mv -v "$f" "$(dirname "$f")/$(basename "$f" | tr '[:upper:]' '[:lower:]')"; done

