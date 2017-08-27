#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <thread>
#include <mutex>

namespace Animate
{
    class AppContext;

    namespace Object::Property
    {
        class Drawable;
    }

    typedef Animate::Object::Property::Drawable Drawable;

    namespace VK
    {
        class Shader;
        class Pipeline;
        class Buffer;
        class Quad;

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
        
        class Context : public std::enable_shared_from_this<Context>
        {
            public:
                Context(std::weak_ptr<Animate::AppContext> context);
                ~Context();
                
                vk::Instance instance;

                VkDebugReportCallbackEXT debug_callback_obj;

                vk::PhysicalDevice physical_device;
                vk::Device logical_device;
                vk::Queue   graphics_queue,
                            present_queue;

                vk::Format depth_format;
                vk::Image depth_image;
                vk::DeviceMemory depth_memory;
                vk::ImageView depth_view;

                vk::SwapchainKHR swap_chain;
                std::vector<vk::Image> swap_chain_images;
                vk::Format swap_chain_image_format;
                vk::Extent2D swap_chain_extent;
                std::vector<vk::ImageView> swap_chain_image_views;
                std::vector<vk::Framebuffer> swap_chain_framebuffers;

                vk::RenderPass render_pass;

                vk::CommandPool command_pool;
                std::vector<vk::CommandBuffer> command_buffers;

                vk::Semaphore image_available_semaphore,
                              render_finished_semaphore;

                vk::DescriptorSetLayout descriptor_set_layout;
                vk::PipelineLayout pipeline_layout;
                vk::DescriptorPool descriptor_pool;

                std::vector<vk::Fence> render_fences;


                //Multisamling targets
                struct {
                    vk::SampleCountFlagBits sample_count = vk::SampleCountFlagBits::e1;
                    struct {
                        vk::Image image;
                        vk::ImageView view;
                        vk::DeviceMemory memory;
                    } colour;
                    struct {
                        vk::Image image;
                        vk::ImageView view;
                        vk::DeviceMemory memory;
                    } depth;
                } multisample_target;

                void fill_command_buffer(int i);

                std::weak_ptr<Pipeline> create_pipeline(
                    std::string fragment_code_id,
                    std::string vertex_code_id,
                    std::vector<std::string> resources = {}
                );

                std::weak_ptr<Buffer> create_buffer(
                    vk::DeviceSize size,
                    vk::BufferUsageFlags usage,
                    vk::MemoryPropertyFlags properties
                );
                
                std::weak_ptr<Buffer> get_buffer(uint64_t id);
                void release_buffer(std::weak_ptr<Buffer> buffer);

                void run_one_time_commands(std::function<void(vk::CommandBuffer)> func);

                void render_scene();
                void commit_scenes();

                void recreate_swap_chain();

                uint32_t find_memory_type(uint32_t type_filter, vk::MemoryPropertyFlags properties);

            private:
                std::weak_ptr<Animate::AppContext> context;

                std::mutex vulkan_resource_mutex;
                std::mutex command_mutex;

                std::vector< std::shared_ptr<Pipeline> > pipelines;
                std::map< uint64_t, std::shared_ptr<Buffer> > buffers;

                vk::DescriptorSet currently_bound_descriptor_set;

                void cleanup_swap_chain_dependancies();

                void create_instance();
                void bind_debug_callback();
                void pick_physical_device();
                void create_logical_device();
                void create_surface();
                void create_swap_chain();
                void create_image_views();
                void create_depth_stencil();
                void create_render_pass();
                void create_multisample_target();
                void create_framebuffers();
                void create_command_pool();
                void create_command_buffers();
                void create_semaphores();
                void create_pipeline_layout();
                void create_uniform_buffer();
                void create_descriptor_pool();
                void create_fences();

                void recreate_pipelines();

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
                vk::SampleCountFlagBits choose_sample_count(VkPhysicalDeviceProperties properties);
                vk::Format choose_depth_format(vk::PhysicalDevice physical_device);

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
