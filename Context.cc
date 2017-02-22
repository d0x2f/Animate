#include "Context.hh"

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
 * Set the GL Area to track.
 *
 * @param gl_area A GL Area
 */
void Context::set_textures(GL::Textures *textures)
{
    this->textures = std::unique_ptr<GL::Textures>(textures);
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


/**
 * Retrieves the tracked GL Textures object.
 */
GL::Textures *Context::get_textures()
{
    if (!this->textures) {
        return NULL;
    }

    return this->textures.get();
}
