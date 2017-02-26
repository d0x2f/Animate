#include "Tile.hh"
#include "../../../GL/Quad.hh"
#include "../../../Geometry/Definitions.hh"

using namespace Animate::Animation::Cat::Object;
using namespace Animate::Geometry;

Tile::Tile(Point position, Scale size)
    : Object(position, size)
{
}

/**
 * Initialise the tile.
 */
void Tile::initialise(Shader *shader, Texture *texture, GLuint position, GLuint grid_size)
{
    //Return if already initialised
    if (this->initialised) {
        g_assert_not_reached();
    }

    this->grid_size = grid_size;

    //Make sure our given position is valid
    if (position > pow(grid_size, 2)-1) {
        g_assert_not_reached();
    }

    //Calculate position
    this->board_position = Point(position % grid_size, position / grid_size);
    this->position.x = this->scale.x * this->board_position.x;
    this->position.y = this->scale.y * this->board_position.y;

    GLfloat grid_size_float = static_cast <GLfloat> (grid_size);

    //Calculate texture position
    Position texture_position = Position(
        this->board_position.x / grid_size_float,
        (1.0 - (1.0/grid_size))-this->board_position.y / grid_size_float
    );
    Position texture_size = Position(
        1./grid_size_float,
        1./grid_size_float
    );

    Quad *quad = new Quad(Point(), Scale(10./grid_size_float, 10./grid_size_float, 1.));
    quad->set_texture_position(texture_position, texture_size);
    quad->initialise(shader, texture);
    this->add_component(quad);

    this->initialised = true;
}

/**
 * Move toward the setposition
 * Reflect off the walls of the screen.
 *
 * @param time_delta The time in microseconds since the last tick.
 */
void Tile::on_tick(GLuint64 time_delta)
{
    GLfloat tile_size = 10./static_cast <GLfloat> (this->grid_size);
    Point position_difference = (this->board_position * tile_size) - this->position;

    if (position_difference.dot(position_difference) > 0.0001) {
        GLfloat move_factor = static_cast <GLfloat> (time_delta)/1000000.;
        Point movement_vector = ((this->board_position * tile_size) - this->position).normalise() * move_factor;
        this->move(movement_vector);
    } else {
        this->set_position(this->board_position * tile_size);
    }
}

void Tile::set_board_position(Position board_position)
{
    //Check that the tile isn't trying to move more than one space
    Position difference = this->board_position.xy() - board_position;
    if ( sqrt(difference.dot(difference)) > 1 ) {
        g_assert_not_reached();
    }

    this->board_position = Point(
        board_position.x,
        board_position.y,
        0.
    );
}
