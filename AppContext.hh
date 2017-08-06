#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <memory>
#include <atomic>

#include "VK/Textures.hh"
#include "VK/Context.hh"

namespace Animate
{
    namespace Animation
    {
        class Animation;
    }

    class AppContext : public std::enable_shared_from_this<AppContext>
    {
        public:
            ~AppContext();

            std::atomic_bool should_close = false;

            void set_window(GLFWwindow *window);
            void set_graphics_context(VK::Context  *graphics_context);
            void set_surface(vk::SurfaceKHR *surface);
            void setup_animations();

            GLFWwindow *get_window();
            std::weak_ptr<vk::SurfaceKHR> const get_surface();
            std::weak_ptr<VK::Context> const get_graphics_context();

            size_t get_animation_count();
            std::weak_ptr<Animation::Animation> get_current_animation();
            void next_animation();

        private:
            GLFWwindow *window;
            std::shared_ptr<vk::SurfaceKHR> surface;
            std::shared_ptr<VK::Context> graphics_context;

            std::shared_ptr<Animation::Animation> noise_animation;
            std::vector< std::shared_ptr<Animation::Animation> >::iterator current_animation;

            std::vector< std::shared_ptr<Animation::Animation> > animations;
    };
}
