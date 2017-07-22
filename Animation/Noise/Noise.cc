#include <gtkmm.h>
#include <iostream>
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
    this->tick_rate = 1;

    srand(time(NULL));
}

/**
 * Perform initialisations.
 */
void Noise::initialise()
{
    std::weak_ptr<VK::Context> graphics_context = this->context.lock()->get_graphics_context();

    //Set shaders
    this->shader = this->context.lock()->get_graphics_context().lock()->create_pipeline("/Animate/data/Noise/shader.frag.spv", "/Animate/data/Noise/shader.vert.spv");

    //Add a Quad
    Object::Object *object = new Object::Object(graphics_context);
    Quad *quad = new Quad(graphics_context);
    quad->initialise(
        this->shader
    );
    object->add_component(quad);
    this->add_object("quad", object);
}

/**
 * Render a frame.
 *
 * @param gl_context    GDK Opengl context reference.
 *
 * @return True so as not to bubble into another render handler.
 */
bool Noise::on_render()
{
    Animation::on_render();

    //Reset matrices
    Matrix model_matrix = Matrix::identity();

    //Look at
    Matrix view_matrix = Matrix::look_at(
        Vector3(0., 0., 1.), // Eye
        Vector3()            // Center (looking at)
    );

    //Ortho
    Matrix projection_matrix = Matrix::orthographic(0, 1, 0, 1, 0, 1);

    this->shader.lock()->set_matrices(model_matrix, view_matrix, projection_matrix);

    //Scoped multex lock
    {
        std::lock_guard<std::mutex> guard(this->tick_mutex);

        this->shader.lock()->set_uniform("random_seed", static_cast <float> (rand()) / static_cast <float> (RAND_MAX));

        //Draw every object
        for (
            std::map< std::string, std::shared_ptr<Animate::Object::Object> >::iterator it = this->objects.begin();
            it != this->objects.end();
            ++it
        ) {
            it->second->draw(model_matrix);
        }
    }
}

/**
 * Compute a tick
 */
void Noise::on_tick(GLuint64 time_delta)
{
    std::lock_guard<std::mutex> guard(this->tick_mutex);

    Animation::on_tick(time_delta);
}
