#pragma once

#include "Shader.hh"
#include "Texture.hh"
#include "../Geometry/Matrix.hh"

using namespace Animate::Geometry;

namespace Animate::GL
{
    class Drawable
    {
        public:
            void initialise(Shader *shader, Texture *texture);
            void set_shader(Shader *shader);
            void set_texture(Texture *texture);

            virtual void initialise_buffers() = 0;
            virtual void draw(Matrix model_matrix) = 0;

        protected:
            bool initialised = false;
            Shader *shader;
            Texture *texture;
    };
}
