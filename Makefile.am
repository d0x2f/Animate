bin_PROGRAMS = animate

animatedir = .
animate_SOURCES =   GL/Area.cc \
                    Gui.cc \
                    Animations/Cat.cc \
                    Animation.cc \
                    main.cc
animate_HEADERS =   GL/Area.hh \
                    Gui.hh \
                    Animations/Cat.hh \
                    Animation.hh

animate_CPPFLAGS = @GTK_CFLAGS@ @GTKMM_CFLAGS@
animate_LDADD = @GTK_LIBS@ @GTKMM_LIBS@

SUBDIRS =   Animations \
            GL

CLEANFILES = *~ 
ACLOCAL_AMFLAGS = -I m4