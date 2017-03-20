bin_PROGRAMS = animate

animatedir = .
animate_SOURCES =   VK/Quad.cc \
                    VK/Circle.cc \
                    VK/Line.cc \
                    VK/Shader.cc \
                    VK/Textures.cc \
                    VK/Texture.cc \
                    \
                    Object/Object.cc \
                    Object/Property/Drawable.cc \
                    Object/Property/Movable.cc \
                    Object/Property/Scalable.cc \
                    Object/Property/Coloured.cc \
                    Object/Property/Rotatable.cc \
                    \
                    Geometry/Matrix.cc \
                    \
                    Animation/Animation.cc \
                    Animation/Cat/Cat.cc \
                    Animation/Cat/Object/Tile.cc \
                    Animation/Modulo/Modulo.cc \
                    Animation/Modulo/Object/Ring.cc \
                    Animation/Noise/Noise.cc \
                    \
                    Gui.cc \
                    Context.cc \
                    resources.c \
                    Utilities.cc \
                    main.cc
animate_HEADERS =   VK/Quad.hh \
                    VK/Circle.hh \
                    VK/Line.hh \
                    VK/Shader.hh \
                    VK/Textures.hh \
                    VK/Texture.hh \
                    \
                    Object/Object.hh \
                    Object/Property/Drawable.hh \
                    Object/Property/Movable.hh \
                    Object/Property/Scalable.hh \
                    Object/Property/Coloured.hh \
                    Object/Property/Rotatable.hh \
                    \
                    Geometry/Matrix.hh \
                    Geometry/Definitions.hh \
                    \
                    Animation/Animation.hh \
                    Animation/Cat/Cat.hh \
                    Animation/Cat/Object/Tile.hh \
                    Animation/Modulo/Modulo.hh \
                    Animation/Modulo/Object/Ring.hh \
                    Animation/Noise/Noise.hh \
                    \
                    Gui.hh \
                    Context.hh \
                    resources.h \
                    Utilities.hh

animate_CPPFLAGS = @GTK_CFLAGS@ @GTKMM_CFLAGS@
animate_LDADD = @GTK_LIBS@ @GTKMM_LIBS@

SUBDIRS =   Animation \
            VK \
            Geometry \
            Object

AM_CXXFLAGS = -g3 -O0
ACLOCAL_AMFLAGS = -I m4

CLEANFILES = *~
