#pragma once

#include "../Animation.hh"
#include "../../VK/Pipeline.hh"
#include "../../Geometry/Definitions.hh"
#include "Object/Tile.hh"

using namespace Animate::Object;
using namespace Animate::Geometry;

namespace Animate::Animation::Casspir
{
    class Casspir : public Animation
    {
        public:
            Casspir(std::weak_ptr<AppContext> context);

            void initialise() override;
            void on_load() override;
            void on_tick(uint64_t time_delta) override;

        protected:
            std::weak_ptr<VK::Pipeline> shader;
            int grid_size = 10;
    };
}
