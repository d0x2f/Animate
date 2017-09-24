#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include "Minesweeper.hh"
#include "../../Utilities.hh"
#include "../../Geometry/Matrix.hh"

using namespace Animate::Animation::Minesweeper;
using namespace Animate::Animation::Minesweeper::Object;

/**
 * Constructor.
 */
Minesweeper::Minesweeper(std::weak_ptr<AppContext> context) : Animation::Animation(context)
{
}

/**
 * Perform initialisations.
 */
void Minesweeper::initialise()
{
    //Set shaders
    this->shader = this->context.lock()->get_graphics_context().lock()->create_pipeline(
        "data/Minesweeper/shader.frag.spv",
        "data/Minesweeper/shader.vert.spv",
        {
            "data/Minesweeper/unflipped.jpg",
            "data/Minesweeper/flipped-0.jpg",
            "data/Minesweeper/flipped-1.jpg",
            "data/Minesweeper/flipped-2.jpg",
            "data/Minesweeper/flipped-3.jpg",
            "data/Minesweeper/flipped-4.jpg",
            "data/Minesweeper/flipped-5.jpg",
            "data/Minesweeper/flipped-6.jpg",
            "data/Minesweeper/flipped-7.jpg",
            "data/Minesweeper/flipped-8.jpg",
            "data/Minesweeper/flagged.jpg",
            "data/Minesweeper/mine-false.jpg",
            "data/Minesweeper/mine-exploded.jpg",
            "data/Minesweeper/mine-reveal.jpg"
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
void Minesweeper::on_load()
{
    Tile *tile;
    std::shared_ptr<Pipeline> pipeline = this->shader.lock();
    std::weak_ptr<VK::Context> graphics_context = this->context.lock()->get_graphics_context();
    for (int i = 0; i < this->grid_size * this->grid_size; i++) {
        tile = new Tile(graphics_context, Point(), Scale(1., 1., 1.));
        tile->initialise(
            pipeline,
            pipeline->get_textures().lock()->get_layer("data/Minesweeper/unflipped.jpg"),
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
void Minesweeper::on_tick(uint64_t time_delta)
{
    Animation::on_tick(time_delta);

    //Update all model matrices
    for(auto const& object: this->objects) {
        object.second->set_model_matrix(Matrix::identity());
    }

}
