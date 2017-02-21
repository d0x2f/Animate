bin_PROGRAMS = animate

animatedir = .
animate_SOURCES =   GL/Area.cc \
                    GL/Quad.cc \
                    GL/Shader.cc \
                    GL/Matrix.cc \
                    Animations/Cat/Cat.cc \
                    Gui.cc \
                    Context.cc \
                    ResourceManager.cc \
                    Animation.cc \
                    Utilities.cc \
                    main.cc
animate_HEADERS =   GL/Area.hh \
                    GL/Quad.hh \
                    GL/Shader.hh \
                    GL/Matrix.hh \
                    GL/Definitions.hh \
                    Animations/Cat/Cat.hh \
                    Gui.hh \
                    Context.hh \
                    ResourceManager.hh \
                    Animation.hh \
                    Utilities.hh

animate_CPPFLAGS = @GTK_CFLAGS@ @GTKMM_CFLAGS@
animate_LDADD = @GTK_LIBS@ @GTKMM_LIBS@

SUBDIRS =   Animations \
            GL

AM_CXXFLAGS = -g3 -O0
ACLOCAL_AMFLAGS = -I m4

CLEANFILES = *~
