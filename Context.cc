#include "Context.hh"

using namespace Animate;

/**
 * Set the texture manager to track.
 *
 * @param textures A textures manager
 */
void Context::set_textures(GL::Textures *textures)
{
    this->textures = std::unique_ptr<GL::Textures>(textures);
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
