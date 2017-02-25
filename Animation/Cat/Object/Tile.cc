#include "Tile.hh"
#include "../../../GL/Quad.hh"
#include "../../../Geometry/Definitions.hh"

using namespace Animate::Animation::Cat::Object;
using namespace Animate::Geometry;

Tile::Tile(Point position, Scale size)
    : Object(position, size)
{
    //Create a random initial vector
    GLfloat max = static_cast <GLfloat> (RAND_MAX);
    this->direction = Vector3(
        ( static_cast <GLfloat> (rand()) / max ) - 0.5,
        ( static_cast <GLfloat> (rand()) / max ) - 0.5
    ).normalise();
}

/**
 * Initialise the tile.
 */
void Tile::initialise(Shader *shader, Texture *texture)
{
    //Return if already initialised
    if (this->initialised) {
        return;
    }

    Quad *quad = new Quad(Point(), Scale(1., 1., 1.));
    quad->initialise(shader, texture);
    this->add_component(quad);

    this->initialised = true;
}

/**
 * Move in the direction vector.
 * Reflect off the walls of the screen.
 *
 * @param time_delta The time in microseconds since the last tick.
 */
void Tile::on_tick(GLuint64 time_delta)
{
    //Move at 10 units per second in the direction of the current vector
    GLfloat move_factor = static_cast <GLfloat> (time_delta) / 100000.;
    Point pos = this->get_position() + (this->direction * move_factor);
    Point diff = (this->direction * move_factor);

    //If this would be out of bounds, then reflect
    Point bottom_left = pos;
    Point top_right = pos + this->get_scale();
    if (
        bottom_left.is_out_of_bounds(Vector2(), Vector2(10, 10)) ||
        top_right.is_out_of_bounds(Vector2(), Vector2(10, 10))
    ) {
        if (bottom_left.x < 0 || top_right.x > 10)
            this->direction.x *= -1;

        if (bottom_left.y < 0 || top_right.y > 10)
            this->direction.y *= -1;

        pos = this->get_position() + (this->direction * move_factor);

        if (this->on_reflect)
            this->on_reflect();
    }

    //Set new position
    this->set_position(pos);
}

void Tile::set_reflect_callback(std::function<void (void)> on_reflect)
{
    this->on_reflect = on_reflect;
}
