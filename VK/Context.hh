#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>

namespace Animate
{
    namespace VK
    {
        class Shader;

        class Context
        {
            public:
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
                vk::PipelineLayout pipeline_layout;
                vk::RenderPass render_pass;
                vk::PipelineCache pipeline_cache;
                vk::Pipeline pipeline;

                std::vector<vk::ShaderModule> shader_modules;
                std::vector<vk::PipelineShaderStageCreateInfo> shader_stages;

                void add_shader_stage(vk::ShaderStageFlagBits type, std::string resource_id);
        };

    }
}
