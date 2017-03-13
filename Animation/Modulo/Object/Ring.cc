#include "Ring.hh"
#include "../../../GL/Circle.hh"
#include "../../../Geometry/Definitions.hh"

using namespace Animate::Animation::Modulo::Object;
using namespace Animate::Geometry;

Ring::Ring(Point position, Scale size)
    : Object(position, size)
{
}

/**
 * Initialise the ring.
 */
void Ring::initialise(Shader *shader)
{
    //Return if already initialised
    if (this->initialised) {
        g_assert_not_reached();
    }

    //Add a circle
    Circle *circle = new Circle(Point(0.5,0.5), Scale(0.5,0.5), Colour(1.,0.,0.,1.), .01);
    circle->initialise(
        shader,
        new Texture()
    );
    this->add_component(circle);

    this->initialised = true;
}

/**
 * Perform a tick.
 *
 * @param time_delta The time in microseconds since the last tick.
 */
void Ring::on_tick(GLuint64 time_delta)
{
}
