#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <map>

#include "../Context.hh"
#include "../Geometry/Definitions.hh"
#include "../Geometry/Matrix.hh"

using namespace Animate::Geometry;

namespace Animate::GL
{
    class Shader
    {
        public:
            Shader(Context *context, std::string fragment_code_id, std::string vertex_code_id);
            ~Shader();

            void initialise();

            void set_matrices(Matrix model, Matrix view, Matrix projection);
            void set_model_matrix(Matrix model);
            void set_uniform(std::string name, Vector4 data);
            void set_uniform(std::string name, GLfloat data);

            void use();
            GLuint get_id() const;

        private:
            Context *context;
            std::string fragment_code_id;
            std::string vertex_code_id;
            GLuint  uniform_buffer_id = 0,
                    matrices_block_index = 0;

            GLuint program_id = 0;

            void compile_shaders();
            void create_uniform_buffer();

            void free();
            void printShaderInfoLog(GLuint obj);
            void printProgramInfoLog(GLuint obj);
    };
}
