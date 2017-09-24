#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include "Casspir.hh"
#include "../../Utilities.hh"
#include "../../Geometry/Matrix.hh"

using namespace Animate::Animation::Casspir;
using namespace Animate::Animation::Casspir::Object;

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
        "data/Casspir/shader.vert.spv",
        {
            "data/Casspir/unflipped.jpg",
            "data/Casspir/flipped-0.jpg",
            "data/Casspir/flipped-1.jpg",
            "data/Casspir/flipped-2.jpg",
            "data/Casspir/flipped-3.jpg",
            "data/Casspir/flipped-4.jpg",
            "data/Casspir/flipped-5.jpg",
            "data/Casspir/flipped-6.jpg",
            "data/Casspir/flipped-7.jpg",
            "data/Casspir/flipped-8.jpg",
            "data/Casspir/flagged.jpg",
            "data/Casspir/mine-false.jpg",
            "data/Casspir/mine-exploded.jpg",
            "data/Casspir/mine-reveal.jpg"
        }
    );

    //Look at
    Matrix view_matrix = Matrix::look_at(
        Vector3(0., 0., 1.), // Eye
        Vector3()            // Center (looking at)
    );

    //Ortho
    Matrix projection_matrix = Matrix::orthographic(0, this->grid_size, 0, this->grid_size, 0, 1);

    this->shader.lock()->set_matrices(view_matrix, projection_matrix);
}


/**
 * Perform functions that should occur before we call ourselves "loaded".
 */
void Casspir::on_load()
{
    Tile *tile;
    std::shared_ptr<Pipeline> pipeline = this->shader.lock();
    std::weak_ptr<VK::Context> graphics_context = this->context.lock()->get_graphics_context();
    for (int i = 0; i < this->grid_size * this->grid_size; i++) {
        tile = new Tile(graphics_context, Point(), Scale(1., 1., 1.));
        tile->initialise(
            pipeline,
            pipeline->get_textures().lock()->get_layer("data/Casspir/unflipped.jpg"),
            i, //position
            this->grid_size  //Grid size
        );

        this->add_object("tile"+std::to_string(i), tile);
    }

    Animation::on_load();
}

/**
 * Compute a tick
 */
void Casspir::on_tick(uint64_t time_delta)
{
    Animation::on_tick(time_delta);

    //Update all model matrices
    for(auto const& object: this->objects) {
        object.second->set_model_matrix(Matrix::identity());
    }

}
