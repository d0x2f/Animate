#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <memory>

#include "VK/VulkanContext.hh"
#include "VK/Textures.hh"

namespace Animate
{
    class Context
    {
        public:
            void set_window(GLFWwindow *window);
            void set_vulkan(VK::VulkanContext *vulkan);
            void set_surface(vk::SurfaceKHR *surface);
            void set_textures(VK::Textures *textures);

            GLFWwindow *get_window();
            std::shared_ptr<vk::SurfaceKHR> const get_surface();
            std::shared_ptr<VK::VulkanContext> const get_vulkan();
            std::shared_ptr<VK::Textures> const get_textures();

        private:
            GLFWwindow *window;
            std::shared_ptr<vk::SurfaceKHR> surface;
            std::shared_ptr<VK::VulkanContext> vulkan;
            std::shared_ptr<VK::Textures> textures;
    };
}
