#pragma once

#include <gtkmm.h>
#include <thread>
#include <mutex>

namespace Animate {

    class Animation {
        public:
            Animation();
            ~Animation();

            virtual bool on_render(const Glib::RefPtr<Gdk::GLContext>& gl_context) {}
            virtual void on_tick() {}

            void run();

            bool check_running();
            int get_tick_rate();

        protected:
            int tick_rate = 60;

        private:
            std::unique_ptr<std::thread> tick_thread;
            std::mutex running_mutex;
            bool running = false;

            static void tick_loop(Animation *animation);
    };

}
