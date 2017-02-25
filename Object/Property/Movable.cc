#include "Movable.hh"

using namespace Animate::Object::Property;
using namespace Animate::Geometry;

/**
 * Constructor
 *
 * @param position The position to begin with.
 */
Movable::Movable(Point position)
    : position(position)
{
}

/**
 * Get position.
 *
 * @return The current position.
 */
Point Movable::get_position()
{
    return this->position;
}

/**
 * Set position.
 *
 * @param position.
 */
void Movable::set_position(Point position)
{
    this->position = position;
}

/**
 * Move the current position by the given delta.
 *
 * @param delta A vector to add to our position.
 */
void Movable::move(Vector3 delta)
{
    this->position += delta;
}
