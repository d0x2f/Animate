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
        glDeleteBuffers(1, &this->ibo_id);
        glDeleteVertexArrays(1, &this->vao_id);
    }
}

/**
 * Initialise the quad.
 */
void Quad::initialise()
{
    //Return if already initialised
    if (this->initialised) {
        return;
    }

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
    //  Point           Normal          Colour
        -5., -5., 0.,     1., 1., 1.,     250., 1., 1., 250.,
        -5.,  5., 0.,     1., 1., 1.,     1., 1., 1., 1.,
         5.,  5., 0.,     1., 1., 1.,     1., 1., 1., 1.,
         5., -5., 0.,     1., 1., 1.,     1., 1., 1., 1.,
    };

    glGenBuffers(1, &this->buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffer_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*28, vertices, GL_STATIC_DRAW);

    glCreateVertexArrays(1, &this->vao_id);
    glBindVertexArray(this->vao_id);

    glBindBuffer(GL_ARRAY_BUFFER, this->buffer_id);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*10, (const void*)0);                     //Position
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*10, (const void*)(sizeof(GLfloat)*3));   //Normal
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*10, (const void*)(sizeof(GLfloat)*6));   //Colour
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*10, (const void*)0);                     //Texture

    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    //Index Data:
    const GLubyte indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    glGenBuffers(1, &this->ibo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte)*6, indices, GL_STATIC_DRAW);

    //Unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/**
 * Bind buffers and draw the quad.
 */
void Quad::draw()
{
    //Bind
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_id);
    glBindVertexArray(this->vao_id);

    //Draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);

    //Unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
