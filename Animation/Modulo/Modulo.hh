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

            bool on_render();
            void on_tick(GLuint64 time_delta);
            void initialise();

        protected:
            std::weak_ptr<VK::Pipeline> shader;
    };
}
