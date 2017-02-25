#pragma once

#include "../../Geometry/Definitions.hh"

using namespace Animate::Geometry;

namespace Animate::Object::Property
{
    class Coloured
    {
        public:
            Coloured(Colour colour);

            Colour get_colour();
            void set_colour(Colour colour);

        protected:
            Colour colour;
    };
}
