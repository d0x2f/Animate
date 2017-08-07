#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <mutex>

#include "Animation/Animation.hh"
#include "AppContext.hh"

using namespace Animate::Animation;

namespace Animate
{
    class Gui
    {
        public:
            Gui();
            ~Gui();

            void start_loops();

            void on_key(int key, int scancode, int action, int mods);
            void on_window_resize(int width, int height);

        private:
            std::shared_ptr<AppContext> context;
            std::thread graphics_thread;

            void init_glfw();
            void init_graphics();
            void init_context();

            void run_tick_loop();
            static void run_graphics_loop(std::shared_ptr<AppContext> app_context);
    };
}
