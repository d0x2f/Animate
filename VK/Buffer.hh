#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include "Context.hh"

using namespace Animate::VK;

namespace Animate::VK
{
    class Buffer
    {
        public:
            Buffer(
                std::weak_ptr<VK::Context> context,
                vk::DeviceSize size,
                vk::BufferUsageFlags usage,
                vk::MemoryPropertyFlags properties
            );
            ~Buffer();

            explicit operator bool() const
            {
                return this->ident;
            }

            operator vk::Buffer() const
            {
                return this->ident;
            }

            uint64_t get_id();

            void copy_buffer_data(Buffer& source);

            void* map();
            void unmap();

            vk::Buffer get_ident();
            vk::DeviceSize get_size();
            vk::BufferUsageFlags get_usage();

        protected:
            std::weak_ptr<VK::Context> context;
            vk::Device logical_device;
            
            vk::Buffer ident;
            vk::DeviceMemory memory;
            vk::DeviceSize size;
            vk::BufferUsageFlags usage;

            static uint64_t id_counter;
            uint64_t id;
    };
}
