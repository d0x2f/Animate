#pragma once

#define VULKAN_HPP_DISABLE_ENHANCED_MODE 1
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>
#include <vector>

#include "../libs/stb_image.h"

namespace Animate::VK
{
    class Context;
    class Buffer;

    struct LayerData {
        size_t size;
        int width;
        int height;
        int channels;
        stbi_uc *pixels;
    };

    class Texture
    {
        public:
            Texture(std::weak_ptr<Context> context, std::vector<std::string> resources);
            ~Texture();

            vk::ImageView get_image_view();
            vk::Sampler get_sampler();

        private:
            std::weak_ptr<Context> context;
            vk::Device logical_device;
            vk::Image image;
            vk::DeviceMemory memory;
            vk::ImageView image_view;
            vk::Sampler sampler;

            void create_image(uint32_t width, uint32_t height, uint32_t layers);
            void transition_image_layout(vk::ImageLayout old_layout, vk::ImageLayout new_layout, uint32_t layers);
            void copy_buffer_to_image(VK::Buffer &staging_buffer, std::vector<LayerData> layers, uint32_t width, uint32_t height);
            void create_image_view(uint32_t layers);
            void create_sampler();

            static std::vector<LayerData> load_resources_as_layers(std::vector<std::string> resources);
    };
}
