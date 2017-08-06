#include <gtkmm-3.0/gtkmm.h>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "Modulo.hh"
#include "Object/Ring.hh"
#include "../../Utilities.hh"
#include "../../Geometry/Definitions.hh"
#include "../../Geometry/Matrix.hh"
#include "../../VK/Circle.hh"

using namespace Animate::Animation::Modulo;
using namespace Animate::Animation::Modulo::Object;
using namespace Animate::Geometry;
using namespace Animate::VK;
using namespace Animate::Object;

/**
 * Constructor.
 * Seed the RNG.
 */
Modulo::Modulo(std::weak_ptr<AppContext> context) : Animation::Animation(context)
{
    srand(time(NULL));
}

/**
 * Perform initialisations.
 */
void Modulo::initialise()
{
    std::weak_ptr<VK::Context> graphics_context = this->context.lock()->get_graphics_context();

    //Set shaders
    this->shader = this->context.lock()->get_graphics_context().lock()->create_pipeline("/Animate/data/Modulo/shader.frag.spv", "/Animate/data/Modulo/shader.vert.spv");

    //Look at
    Matrix view_matrix = Matrix::look_at(
        Vector3(0., 0., 1.), // Eye
        Vector3()            // Center (looking at)
    );

    //Ortho
    Matrix projection_matrix = Matrix::orthographic(0, 1, 0, 1, 0, 1);

    this->shader.lock()->set_matrices(view_matrix, projection_matrix);

    //Add a circle
    Ring *ring = new Ring(graphics_context, Point(0.5,0.5), Scale(.8,.8));
    ring->initialise(
        this->shader.lock()
    );
    this->add_object("ring", ring);
}

/**
 * Compute a tick
 */
void Modulo::on_tick(uint64_t time_delta)
{
    //Increment the hue
    this->hue = fmod(this->hue + (time_delta/10000000.), 6.);
    this->shader.lock()->set_uniform_float(this->hue);

    //Draw every object
    for(auto const& object: this->objects) {
        object.second->set_model_matrix(Matrix::identity());
    }

    Animation::on_tick(time_delta);
}
