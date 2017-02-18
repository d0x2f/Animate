#pragma once

#include <gtkmm.h>
#include <thread>
#include <mutex>

#include "GL/Area.hh"

namespace Animate {

    struct Colour {
        
        float r,g,b,a;

        Colour(float r=0., float g=0., float b=0., float a=1.) : r(r), g(g), b(b), a(a) {}
    };

    class Animation {
        public:
            Animation(GL::Area *gl_area);
            ~Animation();

            virtual bool on_render(const Glib::RefPtr<Gdk::GLContext>& gl_context) {}
            virtual void on_tick() {}

            void run();

            bool check_running();
            int get_tick_rate();

        protected:
            int tick_rate = 60;
            GL::Area *gl_area;

        private:
            std::unique_ptr<std::thread> tick_thread;
            std::mutex running_mutex;
            bool running = false;

            static void tick_loop(Animation *animation);
    };

}
