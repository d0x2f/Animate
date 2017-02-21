#include "Context.hh"
#include "GL/Area.hh"

using namespace Animate;

/**
 * Set the GL Area to track.
 *
 * @param gl_area A GL Area
 */
void Context::set_gl_area(GL::Area *gl_area)
{
    this->gl_area = std::unique_ptr<GL::Area>(gl_area);
}

/**
 * Retrieves the tracked GL Area object.
 */
GL::Area *Context::get_gl_area()
{
    if (!this->gl_area) {
        return NULL;
    }

    return this->gl_area.get();
}
