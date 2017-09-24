#include "Tile.hh"
#include "../../../VK/Quad.hh"
#include "../../../VK/Pipeline.hh"
#include "../../../VK/Texture.hh"
#include "../../../Geometry/Definitions.hh"

using namespace Animate::Animation::Casspir::Object;
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

    //Make sure our given position is valid
    if (position > pow(grid_size, 2)-1) {
        throw std::runtime_error("Invalid tile position given.");
    }

    //Calculate position
    this->position = Point(
        position % grid_size,
        position / grid_size
    );

    float grid_size_float = static_cast <float> (grid_size);

    //Calculate texture position
    Vector3 texture_position = Vector3(
        0,
        0,
        texture_layer
    );
    Vector3 texture_size = Vector3(
        1,
        1
    );

    std::shared_ptr<Quad> quad(new Quad(this->context, Point(), Scale(1., 1., 1.)));
    quad->set_texture_position(texture_position, texture_size);
    quad->initialise(shader);
    this->add_component(quad);

    this->initialised = true;
}

/**
 *
 * @param time_delta The time in microseconds since the last tick.
 */
void Tile::on_tick(uint64_t time_delta)
{
}
