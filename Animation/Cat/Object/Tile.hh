#pragma once

#include "../../../Object/Object.hh"

using namespace Animate::Object;

namespace Animate::Animation::Cat::Object
{
    class Tile : public Animate::Object::Object
    {
        public:
            Tile(std::weak_ptr<VK::Context> context, Point position, Scale size);

            void initialise(std::weak_ptr<Pipeline> shader, std::weak_ptr<Texture> texture, GLuint position, GLuint grid_size);
            void on_tick(GLuint64 time_delta) override;
            void set_board_position(Position board_position);
            void move_to_board_position(Position board_position);

            bool is_moving();

        protected:
            Point board_position;
            GLuint grid_size;
            Point last_movement_vector;
            bool moving = false;
    };
}
