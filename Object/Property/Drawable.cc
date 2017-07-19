#include "Drawable.hh"

using namespace Animate::Object::Property;
using namespace Animate::VK;

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


/**
 * Add this drawable to the scene
 *
 * @param model_matrix the current model_matrix to manipulate for sizing and positioning.
 * @param context the graphics context to add this drawable to
 */
void Drawable::draw(Matrix model_matrix)
{
    this->context->add_to_scene(this);
}