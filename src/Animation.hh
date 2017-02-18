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
            

            static void tick_loop(Animation *animation);

        private:
            std::unique_ptr<std::thread> tick_thread;
            std::mutex running_mutex;
            bool running = false;
    };

}
