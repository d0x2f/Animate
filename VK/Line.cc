#include <algorithm>
#include <iostream>

#include "Line.hh"
#include "Context.hh"
#include "Buffer.hh"
#include "../Geometry/Vertex.hh"

using namespace Animate::VK;

//Reuse existing buffers since for a line, they won't change
uint64_t Line::vertex_buffer_id = 0;
uint64_t Line::index_buffer_id = 0;

/**
 * Constructor
 */
Line::Line(std::weak_ptr<VK::Context> context, Point position, Scale scale, Vector3 rotation, Colour colour, float thickness)
    : Drawable(context, LINE, 4), Movable(position), Scalable(scale), Rotatable(rotation), Coloured(colour)
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
Line::~Line()
{
}


/**
 * Initialise the buffer objects and vertex arrays.
 */
void Line::initialise_buffers()
{
    this->create_vertex_buffer();
    this->create_index_buffer();
}

void Line::create_vertex_buffer()
{
    std::shared_ptr<Context> context = this->context.lock();

    //Check if the buffer is already initialised
    if (this->vertex_buffer.lock()) {
        return;
    }

    if (Line::vertex_buffer_id > 0) {
        this->vertex_buffer = context->get_buffer(Line::vertex_buffer_id);
        return;
    }

    //Vertex & colour Data:
    const Vertex vertices[] = {
    //  Point                                       Texture            Normal               Colour
        Vertex(Vector3(-this->thickness/2, 0., 0.), Vector2(0., 0.), Vector3(0., 0., 1.), Vector4(1., 1., 1., 1.)),
        Vertex(Vector3(this->thickness/2, 0., 0.),  Vector2(0., 0.), Vector3(0., 0., 1.), Vector4(1., 1., 1., 1.)),
        Vertex(Vector3(-this->thickness/2, 1., 0.), Vector2(0., 0.), Vector3(0., 0., 1.), Vector4(1., 1., 1., 1.)),
        Vertex(Vector3(this->thickness/2, 1., 0.),  Vector2(0., 0.), Vector3(0., 0., 1.), Vector4(1., 1., 1., 1.))
    };

    
    vk::DeviceSize size = 16 * sizeof(Vertex);

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

    std::shared_ptr<Buffer> vertex_buffer = this->vertex_buffer.lock();

    vertex_buffer->copy_buffer_data(staging_buffer);

    Line::vertex_buffer_id = vertex_buffer->get_id();
}

void Line::create_index_buffer()
{
    std::shared_ptr<Context> context = this->context.lock();

    //Check if the buffer is already initialised
    if (this->index_buffer.lock()) {
        return;
    }

    if (Line::index_buffer_id > 0) {
        this->index_buffer = context->get_buffer(Line::index_buffer_id);
        return;
    }

    const uint16_t indices[] = {
        0, 2, 1, 3
    };

    vk::DeviceSize size = 4 * sizeof(uint16_t);

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

    std::shared_ptr<Buffer> index_buffer = this->index_buffer.lock();

    index_buffer->copy_buffer_data(staging_buffer);

    Line::index_buffer_id = index_buffer->get_id();
}

vk::Buffer const Line::get_vertex_buffer()
{
    return *this->vertex_buffer.lock().get();
}

vk::Buffer const Line::get_index_buffer()
{
    return *this->index_buffer.lock().get();
}

/**
 * @param model_matrix the current model_matrix to manipulate for sizing and positioning.
 */
void Line::set_model_matrix(Matrix model_matrix)
{
    //Calculate the matrix transform
    Drawable::set_model_matrix(
        model_matrix *
        Matrix::identity()
            .scale(this->scale)
            .rotate(this->rotation)
            .translate(this->position)
    );
}