#include <stdlib.h>

#include "Quad.hh"
#include "../Geometry/Vertex.hh"
#include "Context.hh"
#include "Buffer.hh"

using namespace Animate::VK;
using namespace Animate::Geometry;

/**
 * Constructor
 */
Quad::Quad(std::weak_ptr<VK::Context> context, Point position, Scale size)
    : Drawable(context, QUAD, 4), Movable(position), Scalable(size)
{}

/**
 * Destructor.
 */
Quad::~Quad()
{
    if (!this->context.expired()) {
        std::shared_ptr<Context> context = this->context.lock();
        context->release_buffer(this->vertex_buffer);
        context->release_buffer(this->index_buffer);
    }
}

void Quad::set_texture_position(Vector3 texture_position, Vector3 texture_size)
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
    if (!this->vertex_buffer.expired()) {
        return;
    }

    Vector3 t = this->texture_position;
    Vector3 u = this->texture_position + this->texture_size;

    //Vertex & colour Data:
    const Vertex vertices[] = {
    //  Point                       Texture                 Normal               Colour
        Vertex(Vector3(0., 0., 0.), Vector3(t.x, u.y, t.z), Vector3(0., 0., 1.), Vector4(1., 1., 1., 1.)),
        Vertex(Vector3(1., 0., 0.), Vector3(u.x, u.y, t.z), Vector3(0., 0., 1.), Vector4(1., 1., 1., 1.)),
        Vertex(Vector3(0., 1., 0.), Vector3(t.x, t.y, t.z), Vector3(0., 0., 1.), Vector4(1., 1., 1., 1.)),
        Vertex(Vector3(1., 1., 0.), Vector3(u.x, t.y, t.z), Vector3(0., 0., 1.), Vector4(1., 1., 1., 1.))
    };

    vk::DeviceSize size = 4 * sizeof(Vertex);

    std::shared_ptr<Context> context = this->context.lock();

    std::weak_ptr<VK::Buffer> _staging_buffer = context->create_buffer(
        size,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
    );
    std::shared_ptr<VK::Buffer> staging_buffer = _staging_buffer.lock();

    void *data = staging_buffer->map();
    memcpy(data, vertices, (size_t) size);
    staging_buffer->unmap();
    
    this->vertex_buffer = context->create_buffer(
        size,
        vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
        vk::MemoryPropertyFlagBits::eDeviceLocal
    );

    this->vertex_buffer.lock()->copy_buffer_data(staging_buffer);

    context->release_buffer(staging_buffer);
}

void Quad::create_index_buffer()
{
    //Check if the buffer is already initialised
    if (!this->index_buffer.expired()) {
        return;
    }

    const uint16_t indices[] = {
        0, 2, 1, 3
    };

    vk::DeviceSize size = 4 * sizeof(uint16_t);

    std::shared_ptr<Context> context = this->context.lock();
    
    std::weak_ptr<VK::Buffer> _staging_buffer = context->create_buffer(
        size,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
    );
    std::shared_ptr<VK::Buffer> staging_buffer = _staging_buffer.lock();

    void *data = staging_buffer->map();
    memcpy(data, indices, (size_t) size);
    staging_buffer->unmap();
    
    this->index_buffer = context->create_buffer(
        size,
        vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
        vk::MemoryPropertyFlagBits::eDeviceLocal
    );

    this->index_buffer.lock()->copy_buffer_data(staging_buffer);
    
    context->release_buffer(staging_buffer);
}

vk::Buffer const Quad::get_vertex_buffer()
{
    return *this->vertex_buffer.lock().get();
}

vk::Buffer const Quad::get_index_buffer()
{
    return *this->index_buffer.lock().get();
}

/**
 * @param model_matrix the current model_matrix to manipulate for sizing and positioning.
 */
void Quad::set_model_matrix(Matrix model_matrix)
{
    //Calculate the matrix transform
    Drawable::set_model_matrix(
        model_matrix *
        Matrix::identity()
            .scale(this->scale)
            .translate(this->position)
    );
}
