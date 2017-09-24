#pragma once

#include <memory>

#include "../../../Object/Object.hh"
#include "../../../VK/Quad.hh"

using namespace Animate::Object;
using namespace Animate::VK;

namespace Animate::Animation::Minesweeper::Object
{
    class Tile : public Animate::Object::Object
    {
        public:
            Tile(std::weak_ptr<Context> context, Point position, Scale size);

            void initialise(
                std::weak_ptr<Pipeline> shader,
                uint32_t texture_layer,
                uint32_t position,
                uint32_t grid_size
            );
            void on_tick(uint64_t time_delta) override;
            void set_layer(uint32_t layer);

        protected:
            std::shared_ptr<Quad> quad;
    };
}
