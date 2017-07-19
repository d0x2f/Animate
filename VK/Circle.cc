#include <algorithm>
#include <iostream>

#include "Circle.hh"

#define PI 3.1415926535897f

/**
 * Constructor
 */
Circle::Circle(std::shared_ptr<VK::Context> context, Point position, Scale size, Colour colour, GLfloat thickness)
    : Drawable(context), Movable(position), Scalable(size), Coloured(colour)
{
    //Clamp thickness 0 <= x <= 1
    //this->thickness = std::clamp(thickness, 0., 1.);
    if (thickness > 1.)
        thickness = 1.;
    else if (thickness < 0.)
        thickness = 0.;

    this->thickness = thickness;
}

/**
 * Destructor.
 * Delete buffers.
 */
Circle::~Circle()
{
}


/**
 * Initialise the buffer objects and vertex arrays.
 */
void Circle::initialise_buffers()
{
    //2 vertices per degree each with 3 floats for position data
    GLuint buffer_size = (361*2) * 3;

    //Vertex & colour Data.
    GLfloat vertices[buffer_size];

    //Fill vertices
    for (GLuint i=0; i<361; i++) {
        //Top vertex
        vertices[(i*6)  ] = cos((((GLfloat)i)*PI)/180.);   //x
        vertices[(i*6)+1] = sin((((GLfloat)i)*PI)/180.);   //y
        vertices[(i*6)+2] = 0.;                            //z

        //Bottom vertex
        vertices[(i*6)+3] = cos((((GLfloat)i)*PI)/180.) * (1. - this->thickness);   //x
        vertices[(i*6)+4] = sin((((GLfloat)i)*PI)/180.) * (1. - this->thickness);   //y
        vertices[(i*6)+5] = 0.;                                                     //z
    }

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
 * Bind buffers and draw the circle.
 *
 * @param model_matrix the current model_matrix to manipulate for sizing and positioning.
 */
void Circle::draw(Matrix model_matrix)
{
    //Calculate the matrix transform
    model_matrix = model_matrix * Matrix::identity().scale(this->scale).translate(this->position);

    //Upload the matrix to the shader
    this->shader->set_model_matrix(model_matrix);

    //Set the colour
    this->shader->set_uniform("colour", this->colour);

    /*
    //Bind
    glBindBuffer(GL_ARRAY_BUFFER, this->buffer_id);
    glBindVertexArray(this->vao_id);

    //Enable and set vertex attributes
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3, (const void*)0);
    */
    //Set the shader
    this->shader->use();

    /*
    //Perform the draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 361*2);
    //Disable attribute pointers
    glDisableVertexAttribArray(0);

    //Unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
    */

    Drawable::draw(model_matrix);
}
