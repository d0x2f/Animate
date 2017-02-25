#include "Coloured.hh"

using namespace Animate::GL;

Coloured::Coloured(Colour colour)
    : colour(colour)
{
}

Colour Coloured::get_colour()
{
    return this->colour;
}

void Coloured::set_colour(Colour colour)
{
    this->colour = colour;
}
