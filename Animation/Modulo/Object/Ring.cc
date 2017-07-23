#include <unistd.h>
#include <iostream>

#include "Ring.hh"
#include "../../../VK/Circle.hh"

#define PI 3.1415926535897f

using namespace Animate::Animation::Modulo::Object;
using namespace Animate::Geometry;

Ring::Ring(std::weak_ptr<VK::Context> context, Point position, Scale size)
    : Object(context, position, size), Coloured(Colour(1,1,1,1))
{
}

/**
 * Initialise the ring.
 */
void Ring::initialise(std::weak_ptr<Pipeline> shader)
{
    //Return if already initialised
    if (this->initialised) {
        return;
    }

    //Add the main circle
    std::shared_ptr<Circle> circle(
        new Circle(
            this->context,
            Point(),
            Scale(0.5,0.5),
            Colour(0.,0.,0.,1.),
            .005
        )
    );
    circle->initialise(
        shader
    );
    this->add_component(circle);

    //Create lines
    for (GLuint f=0; f < this->modulo; f++) {
        std::shared_ptr<Line> line(
            new Line(
                this->context,
                Point(),
                Scale(1.,0.5),
                Vector3(0.,0.,PI/2),
                Colour(1.,0.,0.,1.),
                0.001
            )
        );
        line->initialise(
            shader
        );
        this->add_component(line);
        this->lines.push_back(line);
    }


    this->initialised = true;
}

/**
 * Perform a tick.
 *
 * @param time_delta The time in microseconds since the last tick.
 */
void Ring::on_tick(GLuint64 time_delta)
{
    this->factor += (static_cast<GLfloat>(time_delta)/4000000);

    //Convert the hue to an rgb value asuming saturation and value are 100%.
    this->hue = fmod(this->hue + (time_delta/10000000.), 6.);
    GLfloat x = 1 - abs(fmod(this->hue, 2.) - 1);
    if (this->hue <= 1) {
        this->colour = Colour(1., x, 0., 1.);
    } else if (this->hue <= 2) {
        this->colour = Colour(x, 1., 0., 1.);
    } else if (this->hue <= 3) {
        this->colour = Colour(0., 1, x, 1.);
    } else if (this->hue <= 4) {
        this->colour = Colour(0, x, 1., 1.);
    } else if (this->hue <= 5) {
        this->colour = Colour(x, 0., 1., 1.);
    } else if (this->hue <= 6) {
        this->colour = Colour(1, 0., x, 1.);
    }

    //Set the calculated colour for all components.
    for (
        std::vector< std::shared_ptr<Drawable> >::iterator it = this->components.begin();
        it != this->components.end();
        ++it
    ) {
        if(Coloured* component = dynamic_cast<Coloured*>((*it).get())) {
            component->set_colour(this->colour);
        }
    }

    //Use the current modulo to determine the appropriate lines.
    GLfloat t;
    GLfloat f_rad, t_rad;
    GLfloat const m_float = static_cast<GLfloat>(this->modulo);

    GLfloat f=0;
    for (GLint i=0; i < this->modulo; f++, i++) {
        t = fmod( f * this->factor, m_float);

        f_rad = (f / m_float) * (2*PI);
        t_rad = (t / m_float) * (2*PI);

        Vector3 from_point(cos(f_rad)/2., sin(f_rad)/2., 0.);
        Vector3 to_point(cos(t_rad)/2., sin(t_rad)/2., 0.);
        Vector3 difference = (to_point - from_point);
        GLfloat length = sqrt(difference.dot(difference));

        this->lines[i]->set_position(from_point);
        this->lines[i]->set_scale(Scale(1., length));
        this->lines[i]->set_rotation(Vector3(0., 0., -(atan2(difference.y, difference.x)+(1.5*PI))));
        this->lines[i]->set_colour(this->colour);
    }
}
