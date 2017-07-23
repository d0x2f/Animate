#include "Drawable.hh"

using namespace Animate::Object::Property;
using namespace Animate::VK;

/**
 * Initialise the drawable.
 */
void Drawable::initialise(std::weak_ptr<Pipeline> shader, std::weak_ptr<Texture> texture)
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
 * Initialise the drawable.
 */
void Drawable::initialise(std::weak_ptr<Pipeline> shader)
{
    //Return if already initialised
    if (this->initialised) {
        return;
    }

    this->set_shader(shader);

    this->initialise_buffers();

    //Set initialised
    this->initialised = true;
}

/**
 * Set the shader.
 *
 * @param shader A new shader to use when drawing.
 */
void Drawable::set_shader(std::weak_ptr<Pipeline> shader)
{
    this->shader = shader;
}

/**
 * Set the texture.
 *
 * @param shader A new texture to use when drawing.
 */
void Drawable::set_texture(std::weak_ptr<Texture> texture)
{
    this->texture = texture;
}

/**
 * return nullptr for drawables that don't use a buffer.
 *
 * @return An empty vector
 **/
std::vector< std::weak_ptr<Buffer> > const Drawable::get_buffers()
{
    return {};
}

std::weak_ptr<Pipeline> const Drawable::get_shader()
{
    return this->shader;
}

std::weak_ptr<Texture> const Drawable::get_texture()
{
    return this->texture;
}

/**
 * Add this drawable to the scene
 *
 * @param model_matrix the current model_matrix to manipulate for sizing and positioning.
 * @param context the graphics context to add this drawable to
 */
void Drawable::draw(Matrix model_matrix)
{
    this->context.lock()->add_to_scene(this->shared_from_this());
}