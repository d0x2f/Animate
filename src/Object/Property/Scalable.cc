#include "Scalable.hh"

using namespace Animate::Object::Property;
using namespace Animate::Geometry;

/**
 * Constructor
 *
 * @param scale The scale to begin with.
 */
Scalable::Scalable(Scale scale)
    : scale(scale)
{
}

/**
 * Get scale.
 *
 * @return The current scale.
 */
Scale Scalable::get_scale()
{
    return this->scale;
}

/**
 * Set scale.
 *
 * @param scale.
 */
void Scalable::set_scale(Scale scale)
{
    this->scale = scale;
}
