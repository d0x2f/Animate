#pragma once

#include <gtkmm.h>
#include <thread>
#include <mutex>
#include <string>
#include <atomic>

#include "../AppContext.hh"
#include "../Object/Object.hh"

namespace Animate::Animation
{
    class Animation
    {
        public:
            Animation(std::weak_ptr<AppContext> context);
            ~Animation();

            virtual bool on_render();
            virtual void on_load();
            virtual void on_tick(GLuint64 time_delta);
            virtual void initialise() = 0;

            void start();
            void stop();

            bool check_running() const;
            bool check_loaded() const;
            int get_tick_rate() const;

        protected:
            std::weak_ptr<AppContext> context;
            std::mutex tick_mutex;
            int tick_rate = 60;
            GLuint64 last_frame_time = 0;
            int frame_count = 0;
            std::map< std::string, std::unique_ptr<Object::Object> > objects;
            std::atomic_bool loaded;

            void add_object(std::string name, Object::Object *object);
            Object::Object *get_object(std::string name);
            void clear_objects();

        private:
            std::shared_ptr<std::thread> tick_thread;
            std::atomic_bool running;

            static void tick_loop(Animation *animation);
    };
}
