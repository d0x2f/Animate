#include "Coloured.hh"

using namespace Animate::Object::Property;
using namespace Animate::Geometry;

/**
 * Constructor
 *
 * @param colour The colour to begin with.
 */
Coloured::Coloured(Colour colour)
    : colour(colour)
{
}

/**
 * Get the colour.
 *
 * @return The current colour.
 */
Colour Coloured::get_colour()
{
    return this->colour;
}

/**
 * Set colour.
 *
 * @param colour.
 */
void Coloured::set_colour(Colour colour)
{
    this->colour = colour;
}
