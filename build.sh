#/bin/sh

autoreconf --install
glib-compile-resources --target=resources.h --generate-header animate.gresource.xml
glib-compile-resources --target=resources.c --generate-source animate.gresource.xml
cd build
../configure
make
#make distclean
