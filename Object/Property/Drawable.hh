#pragma once

#include "../../VK/Shader.hh"
#include "../../VK/Texture.hh"
#include "../../VK/Context.hh"
#include "../../VK/Buffer.hh"
#include "../Geometry/Matrix.hh"

using namespace Animate::Geometry;
using namespace Animate::VK;

namespace Animate::Object::Property
{
    class Drawable
    {
        public:
            Drawable(std::weak_ptr<VK::Context> context) : context(context) {};
            virtual ~Drawable() {};

            void initialise(Shader *shader, Texture *texture);
            void set_shader(Shader *shader);
            void set_texture(Texture *texture);
            virtual std::weak_ptr<Buffer> const get_buffer();

            virtual void draw(Matrix model_matrix);

        protected:
            std::weak_ptr<VK::Context> context;
            bool initialised = false;
            Shader *shader;
            Texture *texture;

            virtual void initialise_buffers() = 0;
    };
}
