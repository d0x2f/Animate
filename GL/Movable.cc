#include "Movable.hh"

using namespace Animate::GL;

Movable::Movable(Point position)
    : position(position)
{
}

Point Movable::get_position()
{
    return this->position;
}

void Movable::set_position(Point position)
{
    this->position = position;
}

void Movable::move(Vector3 delta)
{
    this->position += delta;
}
