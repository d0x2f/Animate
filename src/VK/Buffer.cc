#include "Buffer.hh"

using namespace Animate::VK;

uint64_t Buffer::id_counter = 1;

Buffer::Buffer(
    std::weak_ptr<VK::Context> _context,
    vk::DeviceSize size,
    vk::BufferUsageFlags usage,
    vk::MemoryPropertyFlags properties
) : context(_context), size(size), usage(usage) {
    std::shared_ptr<Context> context = this->context.lock();

    this->logical_device = context->logical_device;

    vk::BufferCreateInfo buffer_create_info = vk::BufferCreateInfo()
        .setSize(size)
        .setUsage(usage)
        .setSharingMode(vk::SharingMode::eExclusive);

    if(context->logical_device.createBuffer(&buffer_create_info, nullptr, &this->ident) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create buffer.");
    }

    vk::MemoryRequirements memory_requirements;
    context->logical_device.getBufferMemoryRequirements(this->ident, &memory_requirements);

    vk::MemoryAllocateInfo allocation_info = vk::MemoryAllocateInfo()
        .setAllocationSize(memory_requirements.size)
        .setMemoryTypeIndex(
            context->find_memory_type(
                memory_requirements.memoryTypeBits,
                properties
            )
        );

    if(context->logical_device.allocateMemory(&allocation_info, nullptr, &this->memory) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't allocate buffer memory.");
    }

    this->id = Buffer::id_counter++;

    this->context.lock()->logical_device.bindBufferMemory(this->ident, this->memory, 0);
}

Buffer::~Buffer()
{
    this->logical_device.waitIdle();
    if (this->ident) {
        this->logical_device.destroyBuffer(this->ident);
    }

    if (this->memory) {
        this->logical_device.freeMemory(this->memory);
    }
}

uint64_t Buffer::get_id()
{
    return this->id;
}

void Buffer::copy_buffer_data(std::shared_ptr<Buffer> source)
{
    std::lock_guard<std::mutex> guard(this->data_mutex);
    
    std::shared_ptr<Context> context = this->context.lock();

    if (source->get_size() > this->size) {
        throw std::runtime_error("Not enough space to copy buffer.");
    }
    
    context->run_one_time_commands([&](vk::CommandBuffer command_buffer){
        vk::BufferCopy copy_details = vk::BufferCopy()
            .setSrcOffset(0)
            .setDstOffset(0)
            .setSize(this->size);

        command_buffer.copyBuffer(source->get_ident(), ident, 1, &copy_details);
    });
}

void* Buffer::map()
{
    this->data_mutex.lock();

    void* data;
    this->context.lock()->logical_device.mapMemory(this->memory, 0, this->size, vk::MemoryMapFlags(), &data);
    return data;
}

void Buffer::unmap()
{
    this->context.lock()->logical_device.unmapMemory(this->memory);

    this->data_mutex.unlock();
}

vk::DeviceSize Buffer::get_size()
{
    return this->size;
}

vk::BufferUsageFlags Buffer::get_usage()
{
    return this->usage;
}

vk::Buffer Buffer::get_ident()
{
    return this->ident;
}