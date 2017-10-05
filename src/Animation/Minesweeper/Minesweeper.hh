#pragma once

#include <queue>

#include <casspir/casspir.hh>

#include "../Animation.hh"
#include "../../VK/Pipeline.hh"
#include "../../Geometry/Definitions.hh"
#include "Object/Tile.hh"

using namespace Animate::Object;
using namespace Animate::Geometry;

namespace Animate::Animation::Minesweeper
{
    class Minesweeper : public Animation
    {
        public:
            Minesweeper(std::weak_ptr<AppContext> context);

            void initialise() override;
            void on_load() override;
            void on_tick(uint64_t time_delta) override;

        protected:
            std::weak_ptr<VK::Pipeline> shader;
            int grid_size = 10;
            std::queue<Casspir::Operation> move_sequence;
            std::shared_ptr<Casspir::Map> map;
            uint64_t time_since_move = 0;

            void reset_puzzle();
            void redraw_tiles();
    };
}
