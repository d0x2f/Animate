#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

namespace Animate::GL
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
