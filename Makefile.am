bin_PROGRAMS = animate

animatedir = .
animate_SOURCES = GLArea.cc Gui.cc animations/Cat.cc Animation.cc main.cc
animate_HEADERS = GLArea.hh Gui.hh animations/Cat.hh Animation.hh
animate_CPPFLAGS = @GTK_CFLAGS@ @GTKMM_CFLAGS@
animate_LDADD = @GTK_LIBS@ @GTKMM_LIBS@

SUBDIRS = animations
CLEANFILES = *~