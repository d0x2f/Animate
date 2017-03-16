#pragma once

#include <gtkmm.h>
#include <thread>
#include <mutex>
#include <string>

#include "../Context.hh"
#include "../Object/Object.hh"

namespace Animate::Animation
{
    class Animation
    {
        public:
            Animation(Context *context);
            ~Animation();

            virtual bool on_render();
            virtual void on_tick(GLuint64 time_delta);
            virtual void initialise() = 0;

            void run();

            bool check_running();
            int get_tick_rate();

        protected:
            Context *context;
            std::mutex tick_mutex;
            int tick_rate = 60;
            GLuint64 last_frame_time = 0;
            int frame_count = 0;
            std::map< std::string, std::unique_ptr<Object::Object> > objects;

            void add_object(std::string name, Object::Object *object);
            Object::Object *get_object(std::string name);
            void clear_objects();

        private:
            std::unique_ptr<std::thread> tick_thread;
            std::mutex running_mutex;
            bool running = false;

            static void tick_loop(Animation *animation);
    };
}
