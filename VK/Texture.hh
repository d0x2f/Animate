#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <string>

namespace Animate::VK
{
    class Texture
    {
        public:
            ~Texture();

            void load_from_resource(std::string resource_id);
            void use();
            uint64_t get_id();

        protected:
            uint64_t texture_id;
    };
}
