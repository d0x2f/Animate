#pragma once

#include "../Animation.hh"
#include "../../VK/Pipeline.hh"
#include "../../Geometry/Definitions.hh"

using namespace Animate::Object;
using namespace Animate::Geometry;

namespace Animate::Animation::Fractal
{
    class Fractal : public Animation
    {
        public:
            Fractal(std::weak_ptr<AppContext> context);

            void initialise() override;
            void on_tick(uint64_t time_delta) override;

        protected:
            std::weak_ptr<VK::Pipeline> shader;
            uint64_t timer = 0;
    };
}
