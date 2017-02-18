#pragma once

#include "../Animation.hh"
#include "../GL/Area.hh"

namespace Animate {

    class Cat : public Animation
    {
        public:
            Cat(GL::Area *gl_area);

            bool on_render(const Glib::RefPtr<Gdk::GLContext>& gl_context);
            void on_tick();
        
        protected:
            std::mutex colour_mutex;
            Colour colour;
    };

}
