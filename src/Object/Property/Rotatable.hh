#pragma once

#include "../../Geometry/Definitions.hh"
#include "../../Geometry/Matrix.hh"

using namespace Animate::Geometry;

namespace Animate::Object::Property
{
    class Rotatable
    {
        public:
            Rotatable(Vector3 rotation);

            Vector3 get_rotation();

            void set_rotation(Vector3 rotation);
            void rotate_x(float delta);
            void rotate_y(float delta);
            void rotate_z(float delta);

        protected:
            Vector3 rotation;
    };
}
