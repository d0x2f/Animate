#pragma once

#include <map>
#include <taquinsolve.hh>
#include <atomic>

#include "../Animation.hh"
#include "../../VK/Quad.hh"
#include "../../VK/Pipeline.hh"
#include "../../Geometry/Definitions.hh"
#include "../../Object/Object.hh"
#include "Object/Tile.hh"

using namespace Animate::Object;
using namespace Animate::Geometry;
using namespace Animate::Animation::Cat::Object;

namespace Animate::Animation::Cat
{
    class Cat : public Animation
    {
        public:
            Cat(std::weak_ptr<AppContext> context);

            void initialise() override;
            void on_load() override;
            void on_tick(uint64_t time_delta) override;

        protected:
            std::weak_ptr<VK::Pipeline> shader;
            std::vector<uint8_t> initial_position;
            std::queue<TaquinSolve::Moves> move_sequence;
            std::map<int, Tile *> tile_position_map;
            int zero_position = 0;
            int grid_size = 4;
            int texture_index = 0;

            void reset_puzzle();
    };
}
