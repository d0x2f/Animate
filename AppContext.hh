#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <memory>

#include "VK/Context.hh"
#include "VK/Textures.hh"

namespace Animate
{
    class AppContext
    {
        public:
            void set_window(GLFWwindow *window);
            void set_graphics_context(VK::Context const graphics_context);
            void set_surface(vk::SurfaceKHR *surface);
            void set_textures(VK::Textures *textures);

            GLFWwindow *get_window();
            std::shared_ptr<vk::SurfaceKHR> const get_surface();
            VK::Context const get_graphics_context();
            std::shared_ptr<VK::Textures> const get_textures();

        private:
            GLFWwindow *window;
            std::shared_ptr<vk::SurfaceKHR> surface;
            VK::Context graphics_context;
            std::shared_ptr<VK::Textures> textures;
    };
}
