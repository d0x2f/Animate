#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <memory>

#include "VK/Textures.hh"
#include "VK/Context.hh"

namespace Animate
{
    class AppContext
    {
        public:
            ~AppContext();

            void set_window(GLFWwindow *window);
            void set_graphics_context(VK::Context  *graphics_context);
            void set_surface(vk::SurfaceKHR *surface);
            void set_textures(VK::Textures *textures);

            GLFWwindow *get_window();
            std::weak_ptr<vk::SurfaceKHR> const get_surface();
            std::weak_ptr<VK::Context> const get_graphics_context();
            std::weak_ptr<VK::Textures> const get_textures();

        private:
            GLFWwindow *window;
            std::shared_ptr<vk::SurfaceKHR> surface;
            std::shared_ptr<VK::Context> graphics_context;
            std::shared_ptr<VK::Textures> textures;
    };
}
