#define VULKAN_HPP_DISABLE_ENHANCED_MODE 1
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <time.h>

#include "Cat.hh"
#include "../../Utilities.hh"
#include "../../Geometry/Definitions.hh"
#include "../../Geometry/Matrix.hh"
#include "../../VK/Context.hh"
#include "../../VK/Pipeline.hh"

using namespace Animate::Animation::Cat;
using namespace Animate::Animation::Cat::Object;
using namespace Animate::Geometry;
using namespace Animate::VK;
using namespace Animate::Object;

/**
 * Constructor.
 * Seed the RNG.
 */
Cat::Cat(std::weak_ptr<AppContext> context) : Animation::Animation(context)
{
    srand(time(NULL));
}

/**
 * Perform initialisations.
 */
void Cat::initialise()
{
    //Set shaders
    this->shader = this->context.lock()->get_graphics_context().lock()->create_pipeline(
        "data/Cat/shader.frag.spv",
        "data/Cat/shader.vert.spv",
        {
            "data/Cat/0.jpg",
            "data/Cat/1.jpg",
            "data/Cat/2.jpg",
            "data/Cat/3.jpg",
            "data/Cat/4.jpg",
            "data/Cat/5.jpg",
            "data/Cat/6.jpg"
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
 * Recreate the appropriate tiles in their new positions, ready to be solved.
 */
void Cat::reset_puzzle()
{
    //Clear tiles
    this->clear_objects();
    this->tile_position_map.clear();

    //Set puzzle values
    std::string texture_name = "data/Cat/" + std::to_string(this->texture_index++) + ".jpg";
    this->texture_index %= 7;

    std::weak_ptr<VK::Context> graphics_context = this->context.lock()->get_graphics_context();

    //Initialise cat tiles
    Tile *tile;
    std::shared_ptr<Pipeline> pipeline = this->shader.lock();
    for (int i = 0; i < this->grid_size * this->grid_size; i++) {
        if (this->initial_position[i] == 0) {
            this->zero_position = i;
            continue;
        }
        tile = new Tile(graphics_context, Point(), Scale(1., 1., 1.));
        tile->initialise(
            pipeline,
            pipeline->get_textures().lock()->get_layer(texture_name),
            this->initial_position[i], //board position
            this->grid_size  //Grid size
        );
        tile->set_board_position(Position(i%this->grid_size, i/this->grid_size));

        this->add_object("tile"+std::to_string(i), tile);
        this->tile_position_map.insert(std::pair<int, Tile *> (i, tile));
    }
}

/**
 * Perform functions that should occur before we call ourselves "loaded".
 */
void Cat::on_load()
{
    if (this->move_sequence.empty()) {
        this->initial_position = taquin_generate_vector(this->grid_size);
        this->move_sequence = taquin_solve(this->initial_position, this->grid_size);

        this->reset_puzzle();
    }
    Animation::on_load();
}

/**
 * Compute a tick
 */
void Cat::on_tick(uint64_t time_delta)
{
    Animation::on_tick(time_delta);

    //Update all model matrices and add all to the scene
    for(auto const& object: this->objects) {
        object.second->set_model_matrix(Matrix::identity());
    }

    //Check if any tiles are moving
    bool in_motion = false;
    for(auto const& object: this->objects) {
        in_motion |= ((Tile *)object.second.get())->is_moving();
    }

    //If tiles are moving, skip
    if (in_motion) {
        return;
    }

    if (this->move_sequence.empty()) {
        this->initial_position = taquin_generate_vector(this->grid_size);
        this->move_sequence = taquin_solve(this->initial_position, this->grid_size);

        this->reset_puzzle();
    }

    TaquinSolve::Moves move = this->move_sequence.front();
    this->move_sequence.pop();

    int from_position = this->zero_position;
    int to_position = 0;
    switch (move) {
        case TaquinSolve::Moves::UP:
            to_position = from_position - this->grid_size;
            break;

        case TaquinSolve::Moves::DOWN:
            to_position = from_position + this->grid_size;
            break;

        case TaquinSolve::Moves::LEFT:
            to_position = from_position - 1;
            break;

        case TaquinSolve::Moves::RIGHT:
            to_position = from_position + 1;
            break;
    }

    std::map<int, Tile *>::iterator it = this->tile_position_map.find(to_position);
    Tile *tile = it->second;
    it->second->move_to_board_position(Position(
        from_position % this->grid_size,
        from_position / this->grid_size
    ));
    this->tile_position_map.erase(it);
    this->tile_position_map.insert(std::pair<int, Tile *>(from_position, tile));

    this->zero_position = to_position;
}
