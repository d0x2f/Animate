#pragma once

#include "Definitions.hh"

namespace Animate::GL
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
