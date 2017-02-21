#pragma once

#include <memory>

#include "GL/Area.hh"

namespace Animate
{
    class Context
    {
        public:
            void set_gl_area(GL::Area *gl_area);

            GL::Area *get_gl_area();

        private:
            std::unique_ptr<GL::Area> gl_area;
    };
}
