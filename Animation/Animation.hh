#pragma once

#include <gtkmm-3.0/gtkmm.h>
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

            virtual void on_load();
            virtual void on_tick(GLuint64 time_delta);
            virtual void initialise() = 0;

            bool check_loaded();
            void unload();

        protected:
            std::weak_ptr<AppContext> context;
            std::map< std::string, std::shared_ptr<Object::Object> > objects;

            void add_object(std::string name, Object::Object *object);
            std::weak_ptr<Object::Object> get_object(std::string name);
            void clear_objects();

        private:
            std::atomic_bool loaded = false;
    };
}
