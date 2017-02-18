#pragma once

#include "../Animation.hh"
#include "../GLArea.hh"

namespace Animate {

    class Cat : public Animation
    {
        public:
            Cat(GLArea *gl_area);

            bool on_render(const Glib::RefPtr<Gdk::GLContext>& gl_context);
            void on_tick();
        
        protected:
            GLArea *gl_area;

            std::mutex colour_mutex;
            Colour colour;
    };

}
