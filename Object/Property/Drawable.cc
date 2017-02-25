#include "Drawable.hh"

using namespace Animate::Object::Property;
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

/**
 * Set the shader.
 *
 * @param shader A new shader to use when drawing.
 */
void Drawable::set_shader(Shader *shader)
{
    this->shader = shader;
}

/**
 * Set the texture.
 *
 * @param shader A new texture to use when drawing.
 */
void Drawable::set_texture(Texture *texture)
{
    this->texture = texture;
}
