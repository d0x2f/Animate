#pragma once

#include <memory>

#include "GL/Textures.hh"

namespace Animate
{
    class Context
    {
        public:
            void set_textures(GL::Textures *textures);

            GL::Textures *get_textures();

        private:
            std::unique_ptr<GL::Textures> textures;
    };
}
