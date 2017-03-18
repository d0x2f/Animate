# Animate
Interesting minimalist animations

## Animations
* Complete
  * Taquin/15 puzzle solving ([video](https://www.youtube.com/watch?v=F2GyDwdp1KU)).
  * Multiplication modulo drawn on a circle ([video](https://www.youtube.com/watch?v=pxVHWqUBAmg)).
  * Full random noise.
* Ideas
  * Mandlebrot drawing.

## Intention

I'm making this collection of OpenGL animations as a way to grow and showcase my C++/OGL abilities.

Writting computer graphics binds two of my favourite subjects, programming and mathematics. In particlar modern C++ and differential geometry.

## TODO
* Write a method to switch between animations
  * I'm thinking of a keypress to "switch channels" with the noise animation in between to mast loading times.
* Move away from GDK resource manager in favor of some other stand alone resource management library.

## Requirememts

* GLEW, GLU, GL
* GTK3, GTKMM3
* [libtaquinsolve](https://github.com/d0x2f/libtaquinsolve)

## Resources

* https://www.youtube.com/watch?v=qhbuKbxJsk8
* https://en.wikipedia.org/wiki/3D_projection
* http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
* https://en.wikipedia.org/wiki/Affine_transformation#Affine_transformation_in_plane_geometry
* https://github.com/GNOME/gtkmm/blob/master/demos/gtk-demo/example_glarea.cc
* https://www.khronos.org/registry/OpenGL-Refpages/gl4/
* https://developer.gnome.org/gtkmm/stable/
