# Animate
Interesting minimalist animations.

Just a collection of animations, press space to "change the channel".

## Animations
* Complete
  * Taquin/15 puzzle solving ([video](https://www.youtube.com/watch?v=F2GyDwdp1KU)).
  * Multiplication modulo drawn on a circle ([video](https://www.youtube.com/watch?v=pxVHWqUBAmg)).
  * Full random noise.
  * Minesweeper ([video](https://youtu.be/qlBwNXP5lfM)).
  * Mandlebrot Set ([video](https://youtu.be/o_jfFCumiGU)).

## Intention

I'm making this collection of Vulkan/OpenGL animations as a way to grow and showcase my C++/GL abilities.

Writting computer graphics binds two of my favourite subjects, programming and mathematics. In particlar modern C++ and differential geometry.

## TODO
* Implement instanced draws, the modulo animation involved 1001 draws per frame, 2 would be nicer.
  * Drawables within a pipeline are already organised into primitive types (Quad/Line/Circle).
  * Using this pre-organisation the pipeline can store all drawable buffers into a single indexed buffer.
  * Calls to drawable's set\_model\_matrix, set\_color, etc. can trigger an update of only the relevant sections of the buffer.

## Build Requirements

* Autotools
* Python 3
* Vulkan SDK and vulkan compatible hardware
* [libtaquinsolve](https://github.com/d0x2f/libtaquinsolve)
* [Casspir](https://github.com/d0x2f/Casspir)

## Resources

* https://www.youtube.com/watch?v=qhbuKbxJsk8
* https://en.wikipedia.org/wiki/3D_projection
* http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
* https://en.wikipedia.org/wiki/Affine_transformation#Affine_transformation_in_plane_geometry
* https://developer.gnome.org/gtkmm/stable/
* https://developer.nvidia.com/open-source-vulkan-c-api
* https://www.khronos.org/registry/vulkan/specs/1.0/apispec.html
* https://vulkan-tutorial.com
* http://paulbourke.net/fractals/mandelbrot/
* https://www.toptal.com/opengl/opengl-tutorial-for-android-building-a-mandelbrot-set-generator