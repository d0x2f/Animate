#!/bin/sh

autoreconf --install;
find ./data -regex ".*shader\.\(frag\|vert\)" -exec glslangValidator -V \{\} -o \{\}.spv  \;
python GenerateResources.py
./configure;
make;
make check;
