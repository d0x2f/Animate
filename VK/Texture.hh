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
            GLuint get_id();

        protected:
            GLuint texture_id;
    };
}
