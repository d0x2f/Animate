#pragma once

#include "../Animation.hh"

namespace Animate {

    class Cat : public Animation
    {
        public:
            void on_render(GtkGLArea *gl_area, GdkGLContext *gl_context);
            void on_tick();
    };

}
