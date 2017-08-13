#!/bin/sh

autoreconf --install;
find ./data -regex ".*shader\.\(frag\|vert\)" -exec glslangValidator -V \{\} -o \{\}.spv  \;
python3 GenerateResources.py
./configure;
make;
make check;
