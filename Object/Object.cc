#include "Object.hh"
#include "../Geometry/Definitions.hh"

using namespace Animate::Object;

Object::Object(std::weak_ptr<VK::Context> context, Point position, Scale size)
    : Drawable(context), Movable(position), Scalable(size)
{}

Object::~Object()
{
    this->clear_components();
}

/**
 * Add a component to those managed by this object.
 *
 * @param component The component to take possesion of.
 */
void Object::add_component(std::shared_ptr<Drawable> component)
{
    this->components.push_back(component);
}

/**
 * Clear all components.
 */
void Object::clear_components()
{
    this->components.clear();
}

/**
 * We can skip initialisation
 */
void Object::initialise()
{
    //Set initialised
    this->initialised = true;
}

/**
 * We have no buffers to initialise.
 */
void Object::initialise_buffers()
{

}

/**
 * Draw all the components in this object.
 *
 * @param model_matrix Transformation context.
 */
void Object::set_model_matrix(Matrix model_matrix)
{
    //Calculate the matrix transform
    model_matrix = model_matrix * Matrix::identity().scale(this->scale).translate(this->position);

    for (
        std::vector< std::shared_ptr<Drawable> >::iterator it = this->components.begin();
        it != this->components.end();
        ++it
    ) {
        (*it)->set_model_matrix(model_matrix);
    }
}

/**
 * Add this drawable to the scene to be rendered
 */
void Object::add_to_scene()
{
    for (
        std::vector< std::shared_ptr<Drawable> >::iterator it = this->components.begin();
        it != this->components.end();
        ++it
    ) {
        this->context.lock()->add_to_scene(*it);
    }
}

/**
 * Actions to perform for each tick.
 *
 * @param time_delta Time since last tick.
 */
void Object::on_tick(GLuint64 time_delta)
{

}
