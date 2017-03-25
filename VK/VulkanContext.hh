#pragma once

#include <vector>
#include <vulkan/vulkan.hpp>

namespace Animate::VK
{
    struct QueueFamilyIndices {
        int graphics_family = -1;

        bool is_complete() {
            return this->graphics_family >= 0;
        }
    };

    class VulkanContext
    {
        public:
            VulkanContext();
            ~VulkanContext();

        private:
            vk::Instance instance;
            vk::PhysicalDevice physical_device;
            vk::Device logical_device;
            vk::Queue graphics_queue;

            void create_instance();
            void bind_debug_callback();
            void pick_physical_device();
            void create_logical_device();

            bool is_device_suitable(vk::PhysicalDevice device);
            QueueFamilyIndices get_device_queue_famlilies(vk::PhysicalDevice device);

            std::vector<const char*> get_required_extensions() const;
            std::vector<vk::ExtensionProperties> get_avalable_extensions() const;
            std::vector<const char*> get_required_layers() const;
            std::vector<vk::LayerProperties> get_available_layers() const;

            bool check_extensions();
            bool check_layers();

            static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
                VkDebugReportFlagsEXT flags,
                VkDebugReportObjectTypeEXT object_type,
                uint64_t object,
                size_t location,
                int32_t code,
                const char* layer_prefix,
                const char* msg,
                void* user_data
            );
    };
}
