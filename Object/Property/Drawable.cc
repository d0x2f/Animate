#include "Drawable.hh"

#include "../../VK/Pipeline.hh"
#include "../../VK/Texture.hh"
#include "../../VK/Context.hh"
#include "../../VK/Buffer.hh"
#include "../../VK/Context.hh"

using namespace Animate::Object::Property;
using namespace Animate::VK;

/**
 * Initialise the drawable.
 */
void Drawable::initialise(std::weak_ptr<Pipeline> pipeline, std::weak_ptr<Texture> texture)
{
    //Return if already initialised
    if (this->initialised) {
        return;
    }

    this->set_pipeline(pipeline);
    this->set_texture(texture);

    this->initialise_buffers();

    //Set initialised
    this->initialised = true;
}

/**
 * Initialise the drawable.
 */
void Drawable::initialise(std::weak_ptr<Pipeline> pipeline)
{
    //Return if already initialised
    if (this->initialised) {
        return;
    }

    this->set_pipeline(pipeline);

    this->initialise_buffers();

    //Set initialised
    this->initialised = true;
}

/**
 * Set the pipeline.
 *
 * @param pipeline A new pipeline to use when drawing.
 */
void Drawable::set_pipeline(std::weak_ptr<Pipeline> pipeline)
{
    this->pipeline = pipeline;
}

/**
 * Set the texture.
 *
 * @param texture A new texture to use when drawing.
 */
void Drawable::set_texture(std::weak_ptr<Texture> texture)
{
    this->texture = texture;
}

/**
 * return nullptr for drawables that don't use a buffer.
 *
 * @return nullptr
 **/
vk::Buffer const Drawable::get_vertex_buffer()
{
    return nullptr;
}

/**
 * return nullptr for drawables that don't use a buffer.
 *
 * @return nullptr
 **/
vk::Buffer const Drawable::get_index_buffer()
{
    return nullptr;
}

uint32_t Drawable::get_index_count()
{
    return this->indices;
}

std::weak_ptr<Pipeline> const Drawable::get_pipeline()
{
    return this->pipeline;
}

std::weak_ptr<Texture> const Drawable::get_texture()
{
    return this->texture;
}

Matrix const Drawable::get_model_matrix()
{
    return this->model_matrix;
}

/**
 * Set the model matrix for this object.
 *
 * @param model_matrix the current model_matrix to manipulate for sizing and positioning.
 */
void Drawable::set_model_matrix(Matrix model_matrix)
{
    this->model_matrix = model_matrix;
}

/**
 * Add this drawable to the scene to be rendered
 */
void Drawable::add_to_scene()
{
    this->pipeline.lock()->add_drawable(this->shared_from_this());
}