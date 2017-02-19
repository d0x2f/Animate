#pragma once

#include <GL/glew.h>
#include <string>
#include <map>

#include "../Context.hh"
#include "Definitions.hh"

namespace Animate::GL {

    class Shader {
        public:
            Shader(Context *context, std::string fragment_code_id, std::string vertex_code_id);
            ~Shader();

            void initialise();

            void set_matrices(Matrix44 model, Matrix44 view, Matrix44 projection);

            void use();
            GLuint get_id();

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
