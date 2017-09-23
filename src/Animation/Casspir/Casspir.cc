#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include "Casspir.hh"
#include "../../Utilities.hh"
#include "../../Geometry/Matrix.hh"

using namespace Animate::Animation::Casspir;

/**
 * Constructor.
 */
Casspir::Casspir(std::weak_ptr<AppContext> context) : Animation::Animation(context)
{
}

/**
 * Perform initialisations.
 */
void Casspir::initialise()
{
    //Set shaders
    this->shader = this->context.lock()->get_graphics_context().lock()->create_pipeline(
        "data/Casspir/shader.frag.spv",
        "data/Casspir/shader.vert.spv"
    );

    //Look at
    Matrix view_matrix = Matrix::look_at(
        Vector3(0., 0., 1.), // Eye
        Vector3()            // Center (looking at)
    );

    //Ortho
    Matrix projection_matrix = Matrix::orthographic(0, 10, 0, 10, 0, 1);

    this->shader.lock()->set_matrices(view_matrix, projection_matrix);
}


/**
 * Perform functions that should occur before we call ourselves "loaded".
 */
void Casspir::on_load()
{
    Animation::on_load();
}

/**
 * Compute a tick
 */
void Casspir::on_tick(uint64_t time_delta)
{
    Animation::on_tick(time_delta);
}
