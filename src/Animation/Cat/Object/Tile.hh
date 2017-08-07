#pragma once

#include "../../../Object/Object.hh"

using namespace Animate::Object;
using namespace Animate::VK;

namespace Animate::Animation::Cat::Object
{
    class Tile : public Animate::Object::Object
    {
        public:
            Tile(std::weak_ptr<Context> context, Point position, Scale size);

            void initialise(std::weak_ptr<Pipeline> shader, uint32_t texture_layer, uint32_t position, uint32_t grid_size);
            void on_tick(uint64_t time_delta) override;
            void set_board_position(Position board_position);
            void move_to_board_position(Position board_position);

            bool is_moving();

        protected:
            Point board_position;
            uint32_t grid_size;
            Point last_movement_vector;
            bool moving = false;
    };
}
