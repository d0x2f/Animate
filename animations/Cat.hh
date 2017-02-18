#pragma once

#include "../Animation.hh"
#include "../GLArea.hh"

namespace Animate {

    struct Colour {
        
        float r,g,b,a;

        Colour(float r=0., float g=0., float b=0., float a=1.) : r(r), g(g), b(b), a(a) {}
    };

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
