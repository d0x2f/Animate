#pragma once

#include "../../Geometry/Definitions.hh"

using namespace Animate::Geometry;

namespace Animate::Object::Property
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
