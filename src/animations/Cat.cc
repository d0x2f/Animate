#include <iostream>
#include <gtkmm.h>

#include "Cat.hh"

using namespace Animate;

/**
 * Render a frame.
 *
 * @param gl_area       GTK OpenGL area reference.
 * @param gl_context    GTK Opengl context reference.
 */
void Cat::on_render(GtkGLArea *gl_area, GdkGLContext *gl_context)
{
    std::cout << "render" << std::endl;
}

/**
 * Compute a tick
 */
void Cat::on_tick()
{
}
