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
    for (uint32_t f=0; f < this->modulo; f++) {
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
void Ring::on_tick(uint64_t time_delta)
{
    this->factor += (static_cast<float>(time_delta)/4000000);

    //Set the calculated colour for all components.
    for(auto const& component: this->components) {
        if(Coloured *_component = dynamic_cast<Coloured*>(component.get())) {
            _component->set_colour(this->colour);
        }
    }

    //Use the current modulo to determine the appropriate lines.
    float t;
    float f_rad, t_rad;
    float const m_float = static_cast<float>(this->modulo);

    float f=0;
    for (int32_t i=0; i < this->modulo; f++, i++) {
        t = fmod( f * this->factor, m_float);

        f_rad = (f / m_float) * (2*PI);
        t_rad = (t / m_float) * (2*PI);

        Vector3 from_point(cos(f_rad)/2., sin(f_rad)/2., 0.);
        Vector3 to_point(cos(t_rad)/2., sin(t_rad)/2., 0.);
        Vector3 difference = (to_point - from_point);
        float length = sqrt(difference.dot(difference));

        this->lines[i]->set_position(from_point);
        this->lines[i]->set_scale(Scale(1., length));
        this->lines[i]->set_rotation(Vector3(0., 0., -(atan2(difference.y, difference.x)+(1.5*PI))));
        this->lines[i]->set_colour(this->colour);
    }
}
