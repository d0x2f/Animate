#include "Drawable.hh"

using namespace Animate::GL;

/**
 * Initialise the drawable.
 */
void Drawable::initialise(Shader *shader, Texture *texture)
{
    //Return if already initialised
    if (this->initialised) {
        return;
    }

    this->set_shader(shader);
    this->set_texture(texture);

    this->initialise_buffers();

    //Set initialised
    this->initialised = true;
}

void Drawable::set_shader(Shader *shader)
{
    this->shader = shader;
}

void Drawable::set_texture(Texture *texture)
{
    this->texture = texture;
}
