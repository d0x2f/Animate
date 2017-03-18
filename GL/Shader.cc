#include <iostream>

#include "Shader.hh"
#include "../Utilities.hh"

using namespace Animate::GL;

/**
 * Constructor.
 */
Shader::Shader(
    Context *context,
    std::string fragment_code_id,
    std::string vertex_code_id
) : context(context), fragment_code_id(fragment_code_id), vertex_code_id(vertex_code_id)
{
}

/**
 * Destructor.
 */
Shader::~Shader()
{
    this->free();
}

/**
 * Delete shader program if it's set.
 */
void Shader::free()
{
    if (this->program_id != 0) {
        glDeleteProgram(this->program_id);
        this->program_id = 0;
    }
}

/**
 * Fetch, compile and link the shader program.
 */
void Shader::initialise()
{
    //Free an existing shader referenced by this object.
    this->free();

    //Compile shaders.
    this->compile_shaders();

    //Create buffer space for matrix uniforms.
    this->create_uniform_buffer();
}

/**
 * Compile shaders.
 */
void Shader::compile_shaders()
{
    //Create shaders
    GLuint frag_id, vert_id;
    frag_id = glCreateShader(GL_FRAGMENT_SHADER);
    vert_id = glCreateShader(GL_VERTEX_SHADER);

    //Load shader code
    const char *f = (const char *)Utilities::get_resource_as_bytes(fragment_code_id);
    const char *v = (const char *)Utilities::get_resource_as_bytes(vertex_code_id);

    glShaderSource(frag_id, 1, &f, NULL);
    glShaderSource(vert_id, 1, &v, NULL);

    //Compile
    glCompileShader(frag_id);
    glCompileShader(vert_id);

    //Print shader debug info
    printShaderInfoLog(frag_id);
    printShaderInfoLog(vert_id);

    //Link shaders into program
    this->program_id = glCreateProgram();
    glAttachShader(this->program_id, frag_id);
    glAttachShader(this->program_id, vert_id);

    glBindAttribLocation(this->program_id, 0, "_vertex");
    glBindAttribLocation(this->program_id, 1, "_normal");
    glBindAttribLocation(this->program_id, 2, "_colour");
    glBindAttribLocation(this->program_id, 3, "_tex_coords");

    glLinkProgram(this->program_id);

    //Print program debug info
    printProgramInfoLog(this->program_id);

    //Shaders can be detached and deleted after linking
    glDetachShader(this->program_id, frag_id);
    glDetachShader(this->program_id, vert_id);

    glDeleteShader(frag_id);
    glDeleteShader(vert_id);

    std::cout << "Created Shader: " << this->program_id << std::endl;
}

/**
 * Create buffer space for matrix uniforms.
 */
void Shader::create_uniform_buffer()
{
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

    std::cout << "Uniform buffer: " << this->uniform_buffer_id << std::endl;
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
    glBindBuffer(GL_UNIFORM_BUFFER, this->uniform_buffer_id);

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
}

/**
 * Upload the given model matrix to the uniform buffer.
 *
 * @param mode The model matrix.
 */
void Shader::set_model_matrix(Matrix model)
{
    //Bind
    glBindBuffer(GL_UNIFORM_BUFFER, this->uniform_buffer_id);

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
}

void Shader::set_uniform(std::string name, Vector4 data)
{
    this->use();
    int ptr = glGetUniformLocation(this->get_id(), name.c_str());
    if(ptr != -1) {
        glUniform4f(ptr, data.x, data.y, data.z, data.w);
    }
    glUseProgram(0);
}

void Shader::set_uniform(std::string name, GLfloat data)
{
    this->use();
    int ptr = glGetUniformLocation(this->get_id(), name.c_str());
    if(ptr != -1) {
        glUniform1f(ptr, data);
    }
    glUseProgram(0);
}

/**
 * Enable this shader as active.
 */
void Shader::use()
{
    glUseProgram(this->program_id);
}

/**
 * Return the program id.
 */
GLuint Shader::get_id()
{
    return this->program_id;
}

/**
 * Print shader debug info.
 */
void Shader::printShaderInfoLog(GLuint obj) {
    int info_log_length = 0;
    int chars_written  = 0;
    char *info_log;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&info_log_length);

    if (info_log_length > 0)
    {
        info_log = (char *)malloc(info_log_length);
        glGetProgramInfoLog(obj, info_log_length, &chars_written, info_log);
        std::cerr << info_log << std::endl;
        std::free(info_log);
    }
}

/**
 * Print program debug info.
 */
void Shader::printProgramInfoLog(GLuint obj) {
    int info_log_length = 0;
    int chars_written  = 0;
    char *info_log;

    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&info_log_length);

    if (info_log_length > 0)
    {
        info_log = (char *)malloc(info_log_length);
        glGetProgramInfoLog(obj, info_log_length, &chars_written, info_log);
        std::cerr << info_log << std::endl;
        std::free(info_log);
    }
}
