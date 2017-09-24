#pragma once

#include "../../../Object/Object.hh"

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

        protected:
    };
}
