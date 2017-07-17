#include <iostream>

#include "Shader.hh"
#include "../Utilities.hh"

using namespace Animate::VK;

/**
 * Constructor.
 */
Shader::Shader(
    std::weak_ptr<Animate::AppContext> context,
    std::string fragment_code_id,
    std::string vertex_code_id
) : context(context), fragment_code_id(fragment_code_id), vertex_code_id(vertex_code_id)
{
    //Upload shader bytecode to the gpu.
    this->upload();

    //Create buffer space for matrix uniforms.
    this->create_uniform_buffer();
}

/**
 * Destructor.
 */
Shader::~Shader()
{
}

/**
 * Compile shaders.
 */
void Shader::upload()
{
}

/**
 * Create buffer space for matrix uniforms.
 */
void Shader::create_uniform_buffer()
{
    /*
    //Create buffer
    glGenBuffers(1, &this->uniform_buffer_id);

    //Bind
    glBindBuffer(GL_UNIFORM_BUFFER, this->uniform_buffer_id);

    //Fill will null
    glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat)*48, NULL, GL_DYNAMIC_DRAW);

    //Get the matrices block index
    this->matrices_block_index = glGetUniformBlockIndex(this->get_id(), "matrices");

    //Use binding point 0
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->uniform_buffer_id);

    //Set matrices block to use binding point 0
    glUniformBlockBinding(this->get_id(), this->matrices_block_index, 0);

    //Unbind
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    */
}

/**
 * Upload the given matrices to the uniform buffer.
 *
 * @param model The model matrix.
 * @param view The view matrix.
 * @param projection The projection matrix.
 */
void Shader::set_matrices(Matrix model, Matrix view, Matrix projection)
{
    //Bind
    this->use();

    /*
    //Storage
    GLint indices[3];
    GLint offset[3];

    //Get indices and offsets
    glGetActiveUniformBlockiv(this->get_id(), this->matrices_block_index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, indices);
    glGetActiveUniformsiv(this->get_id(), 3, (GLuint *)indices, GL_UNIFORM_OFFSET, offset);

    //Get raw c array
    //Transpose for row_major storage
    GLfloat *model_raw = model.transpose().get_raw_data();
    GLfloat *view_raw = view.transpose().get_raw_data();
    GLfloat *projection_raw = projection.transpose().get_raw_data();

    //Upload
    glBufferSubData(GL_UNIFORM_BUFFER, offset[0], sizeof(GLfloat)*16, model_raw);
    glBufferSubData(GL_UNIFORM_BUFFER, offset[1], sizeof(GLfloat)*16, projection_raw);
    glBufferSubData(GL_UNIFORM_BUFFER, offset[2], sizeof(GLfloat)*16, view_raw);

    //Unbind
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    //Free memory
    std::free(model_raw);
    std::free(view_raw);
    std::free(projection_raw);
    */
}

/**
 * Upload the given model matrix to the uniform buffer.
 *
 * @param mode The model matrix.
 */
void Shader::set_model_matrix(Matrix model)
{
    /*
    //Bind
    this->use();

    //Storage
    GLint indices[3];
    GLint offset[3];

    //Get indices and offsets
    glGetActiveUniformBlockiv(this->get_id(), this->matrices_block_index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, indices);
    glGetActiveUniformsiv(this->get_id(), 3, (GLuint *)indices, GL_UNIFORM_OFFSET, offset);

    //Get raw c array
    GLfloat *model_raw = model.transpose().get_raw_data();

    //Upload
    glBufferSubData(GL_UNIFORM_BUFFER, offset[0], sizeof(GLfloat)*16, model_raw);

    //Unbind
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    //Free memory
    std::free(model_raw);
    */
}

void Shader::set_uniform(std::string name, Vector4 data)
{
    /*
    this->use();
    int ptr = glGetUniformLocation(this->get_id(), name.c_str());
    if(ptr != -1) {
        glUniform4f(ptr, data.x, data.y, data.z, data.w);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glUseProgram(0);
    */
}

void Shader::set_uniform(std::string name, GLfloat data)
{
    /*
    this->use();
    int ptr = glGetUniformLocation(this->get_id(), name.c_str());
    if(ptr != -1) {
        glUniform1f(ptr, data);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glUseProgram(0);
    */
}

/**
 * Enable this shader as active.
 */
void Shader::use()
{
    //glBindBufferBase(GL_UNIFORM_BUFFER, 0, this->uniform_buffer_id);
    //glUseProgram(this->get_id());
}

/**
 * Return the program id.
 */
GLuint Shader::get_id() const
{
}
