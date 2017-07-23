#include <stdlib.h>

#include "Quad.hh"
#include "../Geometry/Vertex.hh"

using namespace Animate::VK;
using namespace Animate::Geometry;

/**
 * Constructor
 */
Quad::Quad(std::weak_ptr<VK::Context> context, Point position, Scale size)
    : Drawable(context), Movable(position), Scalable(size)
{}

/**
 * Destructor.
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
    this->create_vertex_buffer();
    this->create_index_buffer();
}

void Quad::create_vertex_buffer()
{
    //Check if the buffer is already initialised
    if (this->vertex_buffer.lock()) {
        return;
    }

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

    vk::DeviceSize size = 16 * sizeof(Vertex);

    VK::Buffer staging_buffer(
        this->context.lock(),
        size,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
    );

    void *data = staging_buffer.map();
    memcpy(data, vertices, (size_t) size);
    staging_buffer.unmap();
    
    this->vertex_buffer = this->context.lock()->create_buffer(
        size,
        vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
        vk::MemoryPropertyFlagBits::eDeviceLocal
    );

    this->vertex_buffer.lock()->copy_buffer_data(staging_buffer);
}

void Quad::create_index_buffer()
{
    //Check if the buffer is already initialised
    if (this->index_buffer.lock()) {
        return;
    }

    const uint16_t indices[] = {
        0, 1, 2, 2, 1, 3
    };

    vk::DeviceSize size = 6 * sizeof(uint16_t);

    VK::Buffer staging_buffer(
        this->context.lock(),
        size,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
    );

    void *data = staging_buffer.map();
    memcpy(data, indices, (size_t) size);
    staging_buffer.unmap();
    
    this->index_buffer = this->context.lock()->create_buffer(
        size,
        vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
        vk::MemoryPropertyFlagBits::eDeviceLocal
    );

    this->index_buffer.lock()->copy_buffer_data(staging_buffer);
}

std::vector< std::weak_ptr<Buffer> > const Quad::get_buffers()
{
    return {this->vertex_buffer, this->index_buffer};
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
    this->shader.lock()->set_model_matrix(model_matrix);

    //Set the texture
    this->texture.lock()->use();

    Drawable::draw(model_matrix);
}
