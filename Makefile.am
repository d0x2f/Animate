bin_PROGRAMS = animate

animatedir = .
animate_SOURCES =   GL/Area.cc \
                    GL/Quad.cc \
                    GL/Shader.cc \
                    GL/Matrix.cc \
                    GL/Textures.cc \
                    GL/Texture.cc \
                    GL/Drawable.cc \
                    GL/Movable.cc \
                    GL/Scalable.cc \
                    GL/Coloured.cc \
                    Animations/Cat/Cat.cc \
                    Gui.cc \
                    Context.cc \
                    resources.c \
                    Animation.cc \
                    Utilities.cc \
                    main.cc
animate_HEADERS =   GL/Area.hh \
                    GL/Quad.hh \
                    GL/Shader.hh \
                    GL/Matrix.hh \
                    GL/Definitions.hh \
                    GL/Textures.hh \
                    GL/Texture.hh \
                    GL/Drawable.hh \
                    GL/Movable.hh \
                    GL/Scalable.hh \
                    GL/Coloured.hh \
                    Animations/Cat/Cat.hh \
                    Gui.hh \
                    Context.hh \
                    resources.h \
                    Animation.hh \
                    Utilities.hh

animate_CPPFLAGS = @GTK_CFLAGS@ @GTKMM_CFLAGS@
animate_LDADD = @GTK_LIBS@ @GTKMM_LIBS@

SUBDIRS =   Animations \
            GL

AM_CXXFLAGS = -g3 -O0
ACLOCAL_AMFLAGS = -I m4

CLEANFILES = *~
