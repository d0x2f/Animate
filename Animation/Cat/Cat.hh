#pragma once

#include <map>
#include <taquinsolve.hh>
#include <atomic>

#include "../Animation.hh"
#include "../../VK/Quad.hh"
#include "../../VK/Shader.hh"
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

            bool on_render() override;
            void on_load() override;
            void on_tick(GLuint64 time_delta) override;
            void initialise() override;

        protected:
            std::shared_ptr<VK::Shader> shader;
            std::vector<uint8_t> initial_position;
            std::queue<TaquinSolve::Moves> move_sequence;
            std::map<int, Tile *> tile_position_map;
            int zero_position = 0;
            int grid_size = 4;
            int texture_index = 0;

            std::atomic_bool reset_puzzle_flag;
            void reset_puzzle();
    };
}
