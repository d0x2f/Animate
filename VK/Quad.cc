#include <stdlib.h>

#include "Quad.hh"
#include "../Geometry/Vertex.hh"

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
    const Vertex vertices[] = {
    //  Point                       Texture            Normal               Colour
        Vertex(Vector3(0., 0., 0.), Vector2(t.x, u.y), Vector3(0., 0., 1.), Vector4(1., 1., 1., 1.)),
        Vertex(Vector3(1., 0., 0.), Vector2(u.x, u.y), Vector3(0., 0., 1.), Vector4(1., 1., 1., 1.)),
        Vertex(Vector3(0., 1., 0.), Vector2(t.x, t.y), Vector3(0., 0., 1.), Vector4(1., 1., 1., 1.)),
        Vertex(Vector3(1., 1., 0.), Vector2(u.x, t.y), Vector3(0., 0., 1.), Vector4(1., 1., 1., 1.))
    };
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

    //Set the texture
    this->texture->use();

    //Set the shader
    this->shader->use();
}
