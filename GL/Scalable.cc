#include "Scalable.hh"

using namespace Animate::GL;

Scalable::Scalable(Scale scale)
    : scale(scale)
{
}

Scale Scalable::get_scale()
{
    return this->scale;
}

void Scalable::set_scale(Scale scale)
{
    this->scale = scale;
}
