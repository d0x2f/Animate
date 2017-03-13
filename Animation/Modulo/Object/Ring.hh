#pragma once

#include "../../../Object/Object.hh"

using namespace Animate::Object;

namespace Animate::Animation::Modulo::Object
{
    class Ring : public Animate::Object::Object
    {
        public:
            Ring(Point position, Scale size);

            void initialise(Shader *shader);
            void on_tick(GLuint64 time_delta) override;
    };
}
