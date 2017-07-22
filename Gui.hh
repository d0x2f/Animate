#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <vector>

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

            void start_loop();

            void on_key(int key, int scancode, int action, int mods);
            void on_window_resize(int width, int height);

        private:
            std::shared_ptr<AppContext> context;

            std::unique_ptr<Animation::Animation> noise_animation;
            std::vector< std::unique_ptr<Animation::Animation> >::iterator current_animation;

            std::vector< std::unique_ptr<Animation::Animation> > animations;

            void init_glfw();
            void init_graphics();
            void init_context();
            void init_animations();

            void set_animation(Animation::Animation *animation);
    };
}
