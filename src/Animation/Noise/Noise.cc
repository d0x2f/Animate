#include <iostream>
#define VULKAN_HPP_DISABLE_ENHANCED_MODE 1
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <time.h>

#include "Noise.hh"
#include "../../Utilities.hh"
#include "../../Geometry/Definitions.hh"
#include "../../Geometry/Matrix.hh"
#include "../../VK/Circle.hh"

using namespace Animate::Animation::Noise;
using namespace Animate::Geometry;
using namespace Animate::VK;
using namespace Animate::Object;

/**
 * Constructor.
 * Seed the RNG.
 */
Noise::Noise(std::weak_ptr<AppContext> context) : Animation::Animation(context)
{
    srand(time(NULL));
}

/**
 * Perform initialisations.
 */
void Noise::initialise()
{
    std::weak_ptr<VK::Context> graphics_context = this->context.lock()->get_graphics_context();

    //Set shaders
    this->shader = this->context.lock()->get_graphics_context().lock()->create_pipeline(
        "data/Noise/shader.frag.spv",
        "data/Noise/shader.vert.spv"
    );

    //Look at
    Matrix view_matrix = Matrix::look_at(
        Vector3(0., 0., 1.), // Eye
        Vector3()            // Center (looking at)
    );

    //Ortho
    Matrix projection_matrix = Matrix::orthographic(0, 1, 0, 1, 0, 1);

    this->shader.lock()->set_matrices(view_matrix, projection_matrix);

    //Add a Quad
    Object::Object *object = new Object::Object(graphics_context);
    std::shared_ptr<Quad> quad(new Quad(graphics_context));
    quad->initialise(
        this->shader
    );
    object->add_component(quad);
    this->add_object("quad", object);
}

/**
 * Compute a tick and update objects
 */
void Noise::on_tick(uint64_t time_delta)
{
    this->shader.lock()->set_uniform_float(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));

    //Draw every object
    for(auto const& object: this->objects) {
        object.second->set_model_matrix(Matrix::identity());
    }

    Animation::on_tick(time_delta);
}
