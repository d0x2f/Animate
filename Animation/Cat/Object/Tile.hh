#pragma once

#include "../../../Object/Object.hh"

using namespace Animate::Object;

namespace Animate::Animation::Cat::Object
{
    class Tile : public Animate::Object::Object
    {
        public:
            Tile(Point position, Scale size);

            void initialise(Shader *shader, Texture *texture, GLuint position, GLuint grid_size);
            void on_tick(GLuint64 time_delta) override;
            void set_reflect_callback(std::function<void (void)> on_reflect);

        protected:
            Vector3 direction;
            std::function<void (void)> on_reflect;
    };
}
