bin_PROGRAMS = animate

animatedir = .
animate_SOURCES =   VK/Context.cc \
                    VK/Quad.cc \
                    VK/Circle.cc \
                    VK/Line.cc \
                    VK/Pipeline.cc \
                    VK/Textures.cc \
                    VK/Texture.cc \
                    VK/Buffer.cc \
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
                    Animation/Casspir/Casspir.cc \
                    Animation/Casspir/Object/Tile.cc \
                    \
                    Gui.cc \
                    AppContext.cc \
                    Utilities.cc \
                    Resources.cc \
                    \
                    main.cc

animate_HEADERS =   VK/Context.hh \
                    VK/Quad.hh \
                    VK/Circle.hh \
                    VK/Line.hh \
                    VK/Pipeline.hh \
                    VK/Textures.hh \
                    VK/Texture.hh \
                    VK/Buffer.hh \
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
                    Geometry/Vertex.hh \
                    \
                    Animation/Animation.hh \
                    Animation/Cat/Cat.hh \
                    Animation/Cat/Object/Tile.hh \
                    Animation/Modulo/Modulo.hh \
                    Animation/Modulo/Object/Ring.hh \
                    Animation/Noise/Noise.hh \
                    Animation/Casspir/Casspir.hh \
                    Animation/Casspir/Object/Tile.hh \
                    \
                    Gui.hh \
                    AppContext.hh \
                    Utilities.hh\
                    Resources.hh \
                    \
                    libs/stb_image.h

SUBDIRS =   Animation \
            VK \
            Geometry \
            Object

AM_CXXFLAGS = -g3 -O2
ACLOCAL_AMFLAGS = -I m4

CLEANFILES = *~