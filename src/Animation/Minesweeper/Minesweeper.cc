#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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
    srand(time(NULL));
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
            0,
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

    this->time_since_move += time_delta;

    if (this->time_since_move > 5000) {
        if (this->move_sequence.empty()) {
            this->reset_puzzle();
        } else {
            Casspir::Operation operation = this->move_sequence.front();
            this->move_sequence.pop();

            switch (operation.type) {
                case Casspir::OperationType::FLIP:
                    this->map->flip(operation.position);
                    break;
                case Casspir::OperationType::FLAG:
                    this->map->flag(operation.position);
                    break;
            }

            this->redraw_tiles();

        }
        this->time_since_move = 0;
    }
}

/**
 * Recreate the appropriate tiles in their new positions, ready to be solved.
 */
void Minesweeper::reset_puzzle()
{
    //Show the completed puzzle for a bit before resetting.
    if (this->map && this->map->get_status() != Casspir::MapStatus::IN_PROGRESS) {
        usleep(100000);
    }

    //Generate new puzzle
    Casspir::Point first_flip = Casspir::Point(
        rand() % this->grid_size,
        rand() % this->grid_size
    );
    this->map = std::make_shared<Casspir::Map>(
        casspir_generate_map(
            this->grid_size,
            this->grid_size,
            70,
            first_flip
        )
    );
    this->move_sequence = casspir_solve(*this->map);
    this->map->reset();
    this->map->flip(first_flip);

    this->redraw_tiles();

}

void Minesweeper::redraw_tiles()
{
    //Lock resources so that no frames are drawn while we're updating.
    std::lock_guard<std::mutex> guard(this->context.lock()->get_graphics_context().lock()->vulkan_resource_mutex);

    std::shared_ptr<Pipeline> pipeline = this->shader.lock();
    for (int i = 0; i < this->grid_size * this->grid_size; i++) {
        std::weak_ptr<Tile> tile = std::static_pointer_cast<Tile>(this->get_object("tile"+std::to_string(i)).lock());
        Casspir::TileState tile_state = this->map->get_tile(i);

        std::string texture_name = "data/Minesweeper/unflipped.jpg";
        if (this->map->get_status() == Casspir::MapStatus::FAILED) {
            if (tile_state.mine) {
                if (tile_state.flagged) {
                    texture_name = "data/Minesweeper/flagged.jpg";
                } else if (tile_state.flipped) {
                    texture_name = "data/Minesweeper/mine-exploded.jpg";
                } else {
                    texture_name = "data/Minesweeper/mine-reveal.jpg";
                }
            } else {
                if (tile_state.flagged) {
                    texture_name = "data/Minesweeper/mine-false.jpg";
                } else  if (tile_state.flipped) {
                    texture_name = "data/Minesweeper/flipped-" + std::to_string(tile_state.value) + ".jpg";
                }
            }
        } else {
            if (tile_state.flipped) {
                texture_name = "data/Minesweeper/flipped-" + std::to_string(tile_state.value) + ".jpg";
            } else if (tile_state.flagged) {
                texture_name = "data/Minesweeper/flagged.jpg";
            }

        }

        tile.lock()->set_layer(pipeline->get_textures().lock()->get_layer(texture_name));
    }
}