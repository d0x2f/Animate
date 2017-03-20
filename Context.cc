#include "Context.hh"

using namespace Animate;

/**
 * Set the texture manager to track.
 *
 * @param textures A textures manager
 */
void Context::set_textures(VK::Textures *textures)
{
    this->textures = std::shared_ptr<VK::Textures>(textures);
}

/**
 * Retrieves the tracked GL Textures object.
 */
VK::Textures *Context::get_textures()
{
    if (!this->textures) {
        return NULL;
    }

    return this->textures.get();
}
