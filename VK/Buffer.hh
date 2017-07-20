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


            void copy_buffer_data(Buffer& source);

            void* map();
            void unmap();

            vk::DeviceSize get_size();

        protected:
            std::weak_ptr<VK::Context> context;
            vk::Buffer ident;
            vk::DeviceMemory memory;
            vk::DeviceSize size;
    };
}
