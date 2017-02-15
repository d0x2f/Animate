#include <iostream>
#include <gtkmm.h>

#include "Cat.hh"

using namespace Animate;

/**
 * Render a frame.
 *
 * @param gl_area       GTK OpenGL area reference.
 * @param gl_context    GTK Opengl context reference.
 *
 * @return True so as not to bubble into another render handler.
 */
void Cat::on_render(GtkGLArea *gl_area, GdkGLContext *gl_context)
{
    std::cout << "render" << std::endl;
}

void Cat::on_tick()
{
}