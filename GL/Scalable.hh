#pragma once

#include "Definitions.hh"

namespace Animate::GL
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
