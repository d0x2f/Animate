#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <string>
#include <map>

#include "../AppContext.hh"
#include "../Geometry/Definitions.hh"
#include "../Geometry/Matrix.hh"

using namespace Animate::Geometry;

namespace Animate::VK
{
    class Shader
    {
        public:
            Shader(std::weak_ptr<Animate::AppContext> context, std::string fragment_code_id, std::string vertex_code_id);
            ~Shader();

            void set_matrices(Matrix model, Matrix view, Matrix projection);
            void set_model_matrix(Matrix model);
            void set_uniform(std::string name, Vector4 data);
            void set_uniform(std::string name, GLfloat data);

            void use();
            GLuint get_id() const;

        private:
            std::weak_ptr<Animate::AppContext> context;
            std::string fragment_code_id;
            std::string vertex_code_id;

            void upload();
            void create_uniform_buffer();
    };
}
