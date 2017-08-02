#pragma once

#include "../Animation.hh"
#include "../../VK/Quad.hh"
#include "../../VK/Pipeline.hh"
#include "../../Geometry/Definitions.hh"
#include "../../Object/Object.hh"

using namespace Animate::Object;
using namespace Animate::Geometry;

namespace Animate::Animation::Modulo
{
    class Modulo : public Animation
    {
        public:
            Modulo(std::weak_ptr<AppContext> context);

            void initialise();
            void on_tick(uint64_t time_delta);

        protected:
            std::weak_ptr<VK::Pipeline> shader;
    };
}
