#include <algorithm>
#include <iostream>

#include "Circle.hh"

#define PI 3.1415926535897f

/**
 * Constructor
 */
Circle::Circle(std::weak_ptr<VK::Context> context, Point position, Scale size, Colour colour, GLfloat thickness)
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
}

/**
 * @param model_matrix the current model_matrix to manipulate for sizing and positioning.
 */
void Circle::set_model_matrix(Matrix model_matrix)
{
    //Calculate the matrix transform
    Drawable::set_model_matrix(
        model_matrix *
        Matrix::identity()
            .scale(this->scale)
            .translate(this->position)
    );
}
