#include <stdlib.h>

#include "Quad.hh"
#include "../Geometry/Vertex.hh"

using namespace Animate::VK;
using namespace Animate::Geometry;

BufferInfo Quad::buffer_info = BufferInfo();

/**
 * Constructor
 */
Quad::Quad(std::shared_ptr<VK::Context> context, Point position, Scale size)
    : Drawable(context), Movable(position), Scalable(size)
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
    //Check if the buffer is already initialised
    if (Quad::buffer_info.ident) {
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

    vk::BufferCreateInfo buffer_create_info = vk::BufferCreateInfo()
        .setSize(16 * sizeof(Vertex))
        .setUsage(vk::BufferUsageFlagBits::eVertexBuffer)
        .setSharingMode(vk::SharingMode::eExclusive);

    if(this->context->logical_device.createBuffer(&buffer_create_info, nullptr, &Quad::buffer_info.ident) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create buffer.");
    }

    vk::MemoryRequirements memory_requirements;
    this->context->logical_device.getBufferMemoryRequirements(Quad::buffer_info.ident, &memory_requirements);

    vk::MemoryAllocateInfo allocation_info = vk::MemoryAllocateInfo()
        .setAllocationSize(memory_requirements.size)
        .setMemoryTypeIndex(
            this->context->find_memory_type(
                memory_requirements.memoryTypeBits,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
            )
        );

    if(this->context->logical_device.allocateMemory(&allocation_info, nullptr, &Quad::buffer_info.memory) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't allocate buffer memory.");
    }

    this->context->logical_device.bindBufferMemory(Quad::buffer_info.ident, Quad::buffer_info.memory, 0);

    void *data;

    this->context->logical_device.mapMemory(Quad::buffer_info.memory, 0, buffer_create_info.size, vk::MemoryMapFlags(), &data);
    memcpy(data, vertices, (size_t) buffer_create_info.size);
    this->context->logical_device.unmapMemory(Quad::buffer_info.memory);
}

BufferInfo const Quad::get_buffer_info()
{
    return Quad::buffer_info;
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

    Drawable::draw(model_matrix);
}
