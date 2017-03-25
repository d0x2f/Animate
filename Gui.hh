#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <vector>

#include "VK/VulkanContext.hh"
#include "Animation/Animation.hh"
#include "Context.hh"

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

        private:
            std::shared_ptr<Context> context;

            std::shared_ptr<Animation::Animation> noise_animation;
            std::vector< std::shared_ptr<Animation::Animation> >::iterator current_animation;

            std::vector< std::shared_ptr<Animation::Animation> > animations;

            void init_glfw();
            void init_vulkan();
            void init_context();
            void init_animations();

            void set_animation(Animation::Animation *animation);
    };
}
