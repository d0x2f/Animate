#include "Rotatable.hh"

using namespace Animate::Object::Property;
using namespace Animate::Geometry;

/**
 * Constructor
 *
 * @param position The position to begin with.
 */
Rotatable::Rotatable(Vector3 rotation)
    : rotation(rotation)
{
}

/**
 * Get rotation.
 *
 * @return The current position.
 */
Vector3 Rotatable::get_rotation()
{
    return this->rotation;
}

/**
 * Set rotation.
 *
 * @param rotation.
 */
void Rotatable::set_rotation(Vector3 rotation)
{
    this->rotation = rotation;
}

/**
 * Rotate the current x rotation by the given delta.
 *
 * @param delta A value to add to our x rotation.
 */
void Rotatable::rotate_x(float delta)
{
    this->rotation.x += delta;
}

/**
 * Rotate the current x rotation by the given delta.
 *
 * @param delta A value to add to our x rotation.
 */
void Rotatable::rotate_y(float delta)
{
    this->rotation.y += delta;
}

/**
 * Rotate the current x rotation by the given delta.
 *
 * @param delta A value to add to our x rotation.
 */
void Rotatable::rotate_z(float delta)
{
    this->rotation.z += delta;
}
