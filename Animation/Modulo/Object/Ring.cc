#include "Ring.hh"
#include "../../../GL/Circle.hh"
#include "../../../GL/Line.hh"
#include "../../../Geometry/Definitions.hh"

#define PI 3.1415926535897f

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
        return;
    }

    //Add the main circle
    Circle *circle = new Circle(Point(), Scale(0.5,0.5), Colour(0.,0.,0.,1.), .01);
    circle->initialise(
        shader,
        new Texture()
    );
    this->add_component(circle);

    Line *line = new Line(Point(), Scale(.1,0.5), Vector3(0.,0.,PI/2), Colour(1.,0.,0.,1.), 0.1);
    line->initialise(
        shader,
        new Texture()
    );

    this->add_component(line);line = new Line(Point(), Scale(.1,0.5), Vector3(0.,0.,PI), Colour(1.,0.,1.,1.), 0.1);
    line->initialise(
        shader,
        new Texture()
    );
    this->add_component(line);

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
