#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include <vector>
#include <memory>

namespace Animate
{
    class Context;

    namespace VK
    {
        struct QueueFamilyIndices {
            int graphics_family = -1;
            int present_family = -1;

            bool is_complete() {
                return this->graphics_family >= 0 && this->present_family >= 0;
            }
        };

        struct SwapChainSupportDetails {
            vk::SurfaceCapabilitiesKHR capabilities;
            std::vector<vk::SurfaceFormatKHR> formats;
            std::vector<vk::PresentModeKHR> present_modes;
        };

        class VulkanContext
        {
            public:
                VulkanContext(std::shared_ptr<Animate::Context> context);
                ~VulkanContext();

            private:
                std::weak_ptr<Animate::Context> context;
                vk::Instance instance;
                vk::PhysicalDevice physical_device;
                vk::Device logical_device;
                vk::Queue   graphics_queue,
                            present_queue;
                vk::SwapchainKHR swap_chain;
                std::vector<vk::Image> swap_chain_images;
                vk::Format swap_chain_image_format;
                vk::Extent2D swap_chain_extent;
                std::vector<vk::ImageView> swap_chain_image_views;

                void create_instance();
                void bind_debug_callback();
                void pick_physical_device();
                void create_logical_device();
                void create_surface();
                void create_swap_chain();
                void create_image_views();

                bool is_device_suitable(vk::PhysicalDevice device);

                QueueFamilyIndices get_device_queue_families(vk::PhysicalDevice device);
                SwapChainSupportDetails get_swap_chain_support(vk::PhysicalDevice device);

                std::vector<const char*> get_required_instance_extensions() const;
                std::vector<vk::ExtensionProperties> get_avalable_instance_extensions() const;
                std::vector<const char*> get_required_instance_layers() const;
                std::vector<vk::LayerProperties> get_available_instance_layers() const;

                std::vector<const char*> get_required_device_extensions() const;
                std::vector<vk::ExtensionProperties> get_available_device_extensions(vk::PhysicalDevice device) const;

                bool check_instance_extensions();
                bool check_instance_layers();
                bool check_device_extensions(vk::PhysicalDevice device);

                vk::SurfaceFormatKHR choose_swap_surface_format(const std::vector<vk::SurfaceFormatKHR>& available_formats) const;
                vk::PresentModeKHR choose_swap_present_mode(const std::vector<vk::PresentModeKHR>& available_present_modes) const;
                vk::Extent2D choose_swap_extent(const vk::SurfaceCapabilitiesKHR& capabilities) const;

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
}
