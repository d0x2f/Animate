bin_PROGRAMS = animate

animatedir = .
animate_SOURCES =   GL/Area.cc \
                    GL/Quad.cc \
                    GL/Shader.cc \
                    GL/Textures.cc \
                    GL/Texture.cc \
                    GL/Drawable.cc \
                    Object/Property/Movable.cc \
                    Object/Property/Scalable.cc \
                    Object/Property/Coloured.cc \
                    Geometry/Matrix.cc \
                    Animation/Animation.cc \
                    Animation/Cat/Cat.cc \
                    Gui.cc \
                    Context.cc \
                    resources.c \
                    Utilities.cc \
                    main.cc
animate_HEADERS =   GL/Area.hh \
                    GL/Quad.hh \
                    GL/Shader.hh \
                    GL/Textures.hh \
                    GL/Texture.hh \
                    GL/Drawable.hh \
                    Object/Property/Movable.hh \
                    Object/Property/Scalable.hh \
                    Object/Property/Coloured.hh \
                    Geometry/Matrix.hh \
                    Geometry/Definitions.hh \
                    Animation/Animation.hh \
                    Animation/Cat/Cat.hh \
                    Gui.hh \
                    Context.hh \
                    resources.h \
                    Utilities.hh

animate_CPPFLAGS = @GTK_CFLAGS@ @GTKMM_CFLAGS@
animate_LDADD = @GTK_LIBS@ @GTKMM_LIBS@

SUBDIRS =   Animation \
            GL \
            Geometry \
            Object

AM_CXXFLAGS = -g3 -O0
ACLOCAL_AMFLAGS = -I m4

CLEANFILES = *~
