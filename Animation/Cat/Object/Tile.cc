#include "Tile.hh"
#include "../../../VK/Quad.hh"
#include "../../../VK/Pipeline.hh"
#include "../../../VK/Texture.hh"
#include "../../../Geometry/Definitions.hh"

using namespace Animate::Animation::Cat::Object;
using namespace Animate::Geometry;

Tile::Tile(std::weak_ptr<VK::Context> context, Point position, Scale size)
    : Object(context, position, size)
{
}

/**
 * Initialise the tile.
 */
void Tile::initialise(std::weak_ptr<Pipeline> shader, uint32_t texture_layer, uint32_t position, uint32_t grid_size)
{
    //Return if already initialised
    if (this->initialised) {
        return;
    }

    this->grid_size = grid_size;

    position --;

    //Make sure our given position is valid
    if (position > pow(grid_size, 2)-1) {
        throw std::runtime_error("Invalid tile position given.");
    }

    //Calculate position
    this->set_board_position(Position(position % grid_size, position / grid_size));

    float grid_size_float = static_cast <float> (grid_size);

    //Calculate texture position
    Vector3 texture_position = Vector3(
        this->board_position.x / grid_size_float,
        this->board_position.y / grid_size_float,
        texture_layer
    );
    Vector3 texture_size = Vector3(
        1./grid_size_float,
        1./grid_size_float
    );

    std::shared_ptr<Quad> quad(new Quad(this->context, Point(), Scale(1., 1., 1.)));
    quad->set_texture_position(texture_position, texture_size);
    quad->initialise(shader);
    this->add_component(quad);

    this->initialised = true;
}

/**
 * Move toward the setposition
 * Reflect off the walls of the screen.
 *
 * @param time_delta The time in microseconds since the last tick.
 */
void Tile::on_tick(uint64_t time_delta)
{
    //Only tick if this tile is moving
    if (!this->moving) {
        return;
    }


    Point to_position(
        this->board_position.x,
        this->grid_size - this->board_position.y - 1.0
    );

    Point position_difference = to_position - this->position;

    if (position_difference.dot(last_movement_vector) > 0.) {
        float move_factor = static_cast <float> (time_delta)/100000.;
        Point movement_vector = position_difference.normalise() * move_factor;
        this->move(movement_vector);
    } else {
        this->set_position(to_position);
        this->moving = false;
    }
}

bool Tile::is_moving()
{
    return this->moving;
}

void Tile::move_to_board_position(Position board_position)
{
    //Check that the tile isn't trying to move more than one space
    Position difference = this->board_position.xy() - board_position;
    if ( sqrt(difference.dot(difference)) > 1 ) {
        throw std::runtime_error("Tried to move a tile more than one space.");
    }

    this->board_position = Point(
        board_position.x,
        board_position.y,
        0.
    );

    Point to_position(
        this->board_position.x,
        this->grid_size - this->board_position.y - 1.0
    );

    this->last_movement_vector = to_position - this->position;

    this->moving = true;
}

void Tile::set_board_position(Position board_position)
{
    this->board_position = Point(
        board_position.x,
        board_position.y,
        0.
    );

    this->position = Point(
        this->board_position.x,
        this->grid_size - this->board_position.y - 1.0
    );
}
