#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <cmath>

#include "Fractal.hh"
#include "../../Utilities.hh"
#include "../../Geometry/Matrix.hh"
#include "../../VK/Quad.hh"

#define PI 3.1415926535897f

using namespace Animate::Animation::Fractal;
using namespace Animate::VK;

/**
 * Constructor.
 */
Fractal::Fractal(std::weak_ptr<AppContext> context) : Animation::Animation(context)
{
}

/**
 * Perform initialisations.
 */
void Fractal::initialise()
{
    std::weak_ptr<VK::Context> graphics_context = this->context.lock()->get_graphics_context();

    //Set shaders
    this->shader = this->context.lock()->get_graphics_context().lock()->create_pipeline(
        "data/Fractal/shader.frag.spv",
        "data/Fractal/shader.vert.spv"
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
    quad->set_buffer_transform(
        Matrix::identity()
            .scale(Vector3(3., 3., 1.))
            .translate(Vector3(-2, -1.5, 0.))
    );
    quad->initialise(
        this->shader
    );
    object->add_component(quad);
    this->add_object("quad", object);
}

/**
 * Compute a tick
 */
void Fractal::on_tick(uint64_t time_delta)
{
    this->timer += time_delta;
    float zoom_factor = (static_cast<float>(this->timer / 10000) / 100.) - 4;
    auto object = this->get_object("quad");

    float theta = zoom_factor;
    float x = 0.25;
    float y = 0.;

    object.lock()->set_model_matrix(
        Matrix::identity()
            .translate(Vector3(-x-0.015, -y-0.003))
            .scale(Vector2(exp(zoom_factor), exp(zoom_factor)))
            .rotate(Vector3(0.,0.,zoom_factor))
            .translate(Vector3(x+0.5, y + 0.5))
    );

    Animation::on_tick(time_delta);
}
