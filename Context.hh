#pragma once

#include <memory>

#include "GL/Area.hh"
#include "GL/Textures.hh"

namespace Animate
{
    class Context
    {
        public:
            void set_gl_area(GL::Area *gl_area);
            void set_textures(GL::Textures *textures);

            GL::Area *get_gl_area();
            GL::Textures *get_textures();

        private:
            std::unique_ptr<GL::Area> gl_area;
            std::unique_ptr<GL::Textures> textures;
    };
}
