#include <iostream>
#include <stdlib.h>

#include "Quad.hh"

using namespace Animate::GL;

/**
 * Constructor
 */
Quad::Quad(Point position, Size size, Colour colour)
    : position(position), size(size), colour(colour)
{}

/**
 * Destructor.
 * Delete buffers.
 */
Quad::~Quad()
{
    if (this->initialised) {
        glDeleteBuffers(1, &this->buffer_id);
        glDeleteVertexArrays(1, &this->vao_id);
    }
}

void Quad::set_shader(Shader *shader)
{
    this->shader = shader;
}

void Quad::set_texture(Texture *texture)
{
    this->texture = texture;
}

/**
 * Initialise the quad.
 */
void Quad::initialise(Shader *shader, Texture *texture)
{
    //Return if already initialised
    if (this->initialised) {
        return;
    }

    this->set_shader(shader);
    this->set_texture(texture);

    this->initialise_buffers();

    //Set initialised
    this->initialised = true;
}
/**
 * Initialise the quads ibo and vbo
 */
void Quad::initialise_buffers()
{
    //Vertex & colour Data:
    const GLfloat vertices[] = {
    //  Point           Texture         Normal          Colour
        0., 0., 0.,     0., 1.,         0., 0., 1.,     1., 1., 1., 1.,
        0., 1., 0.,     0., 0.,         0., 0., 1.,     1., 1., 1., 1.,
        1., 1., 0.,     1., 0.,         0., 0., 1.,     1., 1., 1., 1.,
        0., 0., 0.,     0., 1.,         0., 0., 1.,     1., 1., 1., 1.,
        1., 1., 0.,     1., 0.,         0., 0., 1.,     1., 1., 1., 1.,
        1., 0., 0.,     1., 1.,         0., 0., 1.,     1., 1., 1., 1.
    };

    //Generate vertex array
    glGenVertexArrays(1, &this->vao_id);
    glBindVertexArray(this->vao_id);

    //Upload vertex data
    glGenBuffers(1, &this->buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffer_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**
 * Bind buffers and draw the quad.
 *
 * @param model_matrix the current model_matrix to manipulate for sizing and positioning.
 */
void Quad::draw(Matrix model_matrix)
{
    //Calculate the matrix transform
    model_matrix = Matrix::identity().scale(this->size).translate(this->position);// * model_matrix;

    //Upload the matrix to the shader
    this->shader->set_model_matrix(model_matrix);

    //Bind
    glBindBuffer(GL_ARRAY_BUFFER, this->buffer_id);
    glBindVertexArray(this->vao_id);

    //Enable and set vertex attributes
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*12, (const void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*12, (const void*)(sizeof(GLfloat)*5));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*12, (const void*)(sizeof(GLfloat)*8));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*12, (const void*)(sizeof(GLfloat)*3));

    //Set the texture
    this->texture->use();

    //Set the shader
    this->shader->use();

    //Perform the draw
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //Disable attribute pointers
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    //Unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Point Quad::get_position()
{
    return this->position;
}

void Quad::set_position(Point position)
{
    this->position = position;
}

void Quad::move(Vector3 delta)
{
    this->position += delta;
}
