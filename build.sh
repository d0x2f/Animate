#!/bin/sh

autoreconf --install;
find ./data -regex ".*shader\.\(frag\|vert\)" -exec glslangValidator -V \{\} -o \{\}.spv  \;
glib-compile-resources --target=src/resources.h --generate-header animate.gresource.xml;
glib-compile-resources --target=src/resources.c --generate-source animate.gresource.xml;
./configure;
make;
make check;
