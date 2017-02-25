#pragma once

#include "../../Geometry/Definitions.hh"

using namespace Animate::Geometry;

namespace Animate::Object::Property
{
    class Scalable
    {
        public:
            Scalable(Scale scale);

            Scale get_scale();
            void set_scale(Scale scale);

        protected:
            Scale scale;
    };
}
