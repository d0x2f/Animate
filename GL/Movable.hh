#pragma once

#include "Definitions.hh"

namespace Animate::GL
{
    class Movable
    {
        public:
            Movable(Point position);

            Point get_position();
            void set_position(Point position);
            void move(Vector3 delta);

        protected:
            Point position;
    };
}
