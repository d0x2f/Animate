#pragma once

#include <gtkmm.h>
#include <thread>
#include <mutex>

#include "GL/Area.hh"
#include "Context.hh"

namespace Animate
{
    class Animation
    {
        public:
            Animation(Context *context);
            ~Animation();

            virtual bool on_render(const Glib::RefPtr<Gdk::GLContext>& gl_context) {}
            virtual void on_tick() {}
            virtual void on_realise() {}

            void run();

            bool check_running();
            int get_tick_rate();

        protected:
            Context *context;
            int tick_rate = 60;

            virtual void initialise() {}

        private:
            std::unique_ptr<std::thread> tick_thread;
            std::mutex running_mutex;
            bool running = false;

            static void tick_loop(Animation *animation);
    };
}
