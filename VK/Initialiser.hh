#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include <vector>
#include <memory>

#include "Context.hh"

namespace Animate
{
    class AppContext;

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

        class Initialiser
        {
            public:
                Initialiser(std::shared_ptr<Animate::AppContext> context);

                Context const create_context();

            private:
                std::weak_ptr<Animate::AppContext> context;

                void create_instance(Context& context);
                void bind_debug_callback(Context& context);
                void pick_physical_device(Context& context);
                void create_logical_device(Context& context);
                void create_surface(Context& context);
                void create_swap_chain(Context& context);
                void create_image_views(Context& context);
                void create_render_pass(Context& context);
                void create_pipeline(Context& context);
                void create_framebuffers(Context& context);
                void create_command_pool(Context& context);
                void create_command_buffers(Context& context);

                bool is_device_suitable(vk::PhysicalDevice const & device);

                QueueFamilyIndices get_device_queue_families(vk::PhysicalDevice const & device);
                SwapChainSupportDetails get_swap_chain_support(vk::PhysicalDevice const & device);

                std::vector<const char*> get_required_instance_extensions() const;
                std::vector<vk::ExtensionProperties> get_avalable_instance_extensions() const;
                std::vector<const char*> get_required_instance_layers() const;
                std::vector<vk::LayerProperties> get_available_instance_layers() const;

                std::vector<const char*> get_required_device_extensions() const;
                std::vector<vk::ExtensionProperties> get_available_device_extensions(vk::PhysicalDevice const & device) const;

                bool check_instance_extensions();
                bool check_instance_layers();
                bool check_device_extensions(vk::PhysicalDevice const & device);

                vk::SurfaceFormatKHR choose_swap_surface_format(std::vector<vk::SurfaceFormatKHR> const & available_formats) const;
                vk::PresentModeKHR choose_swap_present_mode(std::vector<vk::PresentModeKHR> const & available_present_modes) const;
                vk::Extent2D choose_swap_extent(vk::SurfaceCapabilitiesKHR const & capabilities) const;

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
