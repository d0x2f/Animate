#include <gtkmm.h>
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
    this->tick_rate = 120;

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


    //Add a circle
    Ring *ring = new Ring(graphics_context, Point(0.5,0.5), Scale(.8,.8));
    ring->initialise(
        this->shader.lock()
    );
    this->add_object("ring", ring);
}

/**
 * Render a frame.
 *
 * @param gl_context    GDK Opengl context reference.
 *
 * @return True so as not to bubble into another render handler.
 */
bool Modulo::on_render()
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
void Modulo::on_tick(GLuint64 time_delta)
{
    std::lock_guard<std::mutex> guard(this->tick_mutex);

    Animation::on_tick(time_delta);
}
