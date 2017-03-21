#include <stdlib.h>

#include "Quad.hh"

using namespace Animate::VK;
using namespace Animate::Geometry;

/**
 * Constructor
 */
Quad::Quad(Point position, Scale size)
    : Movable(position), Scalable(size)
{}

/**
 * Destructor.
 * Delete buffers.
 */
Quad::~Quad()
{
}

void Quad::set_texture_position(Position texture_position, Position texture_size)
{
    this->texture_position = texture_position;
    this->texture_size = texture_size;
}

/**
 * Initialise the quads ibo and vbo
 */
void Quad::initialise_buffers()
{
    Vector2 t = this->texture_position;
    Vector2 u = this->texture_position + this->texture_size;

    //Vertex & colour Data:
    const GLfloat vertices[] = {
    //  Point           Texture         Normal          Colour
        0., 0., 0.,     t.x, u.y,         0., 0., 1.,     1., 1., 1., 1.,
        0., 1., 0.,     t.x, t.y,         0., 0., 1.,     1., 1., 1., 1.,
        1., 1., 0.,     u.x, t.y,         0., 0., 1.,     1., 1., 1., 1.,
        0., 0., 0.,     t.x, u.y,         0., 0., 1.,     1., 1., 1., 1.,
        1., 1., 0.,     u.x, t.y,         0., 0., 1.,     1., 1., 1., 1.,
        1., 0., 0.,     u.x, u.y,         0., 0., 1.,     1., 1., 1., 1.
    };

    /*
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
    */
}

/**
 * Bind buffers and draw the quad.
 *
 * @param model_matrix the current model_matrix to manipulate for sizing and positioning.
 */
void Quad::draw(Matrix model_matrix)
{
    //Calculate the matrix transform
    model_matrix = model_matrix * Matrix::identity().scale(this->scale).translate(this->position);

    //Upload the matrix to the shader
    this->shader->set_model_matrix(model_matrix);

    /*
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
    */
    //Set the texture
    this->texture->use();

    //Set the shader
    this->shader->use();

    /*
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
    */
}
