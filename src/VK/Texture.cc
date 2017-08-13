#include <iostream>

#include "Texture.hh"
#include "Context.hh"
#include "Buffer.hh"
#include "../Utilities.hh"

#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"

using namespace Animate::VK;

Texture::Texture(std::weak_ptr<Context> context, std::vector<std::string> resources) : context(context)
{
    this->logical_device = context.lock()->logical_device;

    std::vector<LayerData> layers = Texture::load_resources_as_layers(resources);

    //Find the total size needed for the staging buffer.
    vk::DeviceSize total_size = 0;
    for(auto const& layer : layers) {
        total_size += layer.size;
    }

    //Create the staging buffer.
    VK::Buffer staging_buffer(
        this->context.lock(),
        total_size,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
    );

    //Extend width & height.
    int width=0, height=0;

    //Copy each image into it.
    size_t offset = 0;
    void *data = staging_buffer.map();
    for(auto const& layer : layers) {
        memcpy(data + offset, layer.pixels, layer.size);
        offset += layer.size;

        width = std::max(width, layer.width);
        height = std::max(height, layer.height);

        //Free pixel memory
        stbi_image_free(layer.pixels);
    }
    staging_buffer.unmap();
    
    this->create_image(width, height, layers.size());
    this->copy_buffer_to_image(staging_buffer, layers, width, height);
    this->create_image_view(layers.size());
    this->create_sampler();

    std::cout << "Loaded texture" << std::endl;
}

Texture::~Texture()
{
    this->logical_device.destroySampler(this->sampler);
    this->logical_device.destroyImageView(this->image_view);
    this->logical_device.destroyImage(this->image);
    this->logical_device.freeMemory(this->memory);
}

std::vector<LayerData> Texture::load_resources_as_layers(std::vector<std::string> resources)
{
    std::vector<LayerData> layers;

    for(auto const& resource_id : resources) {
        LayerData layer;

        size_t data_length;
        uint8_t const *data = reinterpret_cast<const uint8_t*>(Utilities::get_resource_as_bytes(resource_id, &data_length));

        layer.pixels = stbi_load_from_memory(
            data,
            static_cast<int>(data_length),
            &layer.width,
            &layer.height,
            &layer.channels,
            STBI_rgb_alpha
        );

        if (!layer.pixels) {
            throw std::runtime_error("Couldn't load texture resource: " + resource_id + " (" + stbi_failure_reason() + ")");
        } else {
            std::cout << "Texture loaded: " + resource_id << std::endl;
        }

        layer.size = layer.width * layer.height * 4;

        layers.push_back(layer);
    }

    return layers;
}

vk::ImageView Texture::get_image_view()
{
    return this->image_view;
}

vk::Sampler Texture::get_sampler()
{
    return this->sampler;
}

void Texture::create_image(uint32_t width, uint32_t height, uint32_t layers)
{
    vk::ImageCreateInfo create_info = vk::ImageCreateInfo()
        .setImageType(vk::ImageType::e2D)
        .setExtent({width, height, 1})
        .setMipLevels(1)
        .setArrayLayers(layers)
        .setFormat(vk::Format::eR8G8B8A8Unorm)
        .setTiling(vk::ImageTiling::eOptimal)
        .setInitialLayout(vk::ImageLayout::ePreinitialized)
        .setUsage(vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled)
        .setSamples(vk::SampleCountFlagBits::e1)
        .setSharingMode(vk::SharingMode::eExclusive);

    std::shared_ptr<Context> context = this->context.lock();

    if (context->logical_device.createImage(&create_info, nullptr, &this->image) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create image.");
    }

    vk::MemoryRequirements memory_requirements;
    context->logical_device.getImageMemoryRequirements(this->image, &memory_requirements);

    vk::MemoryAllocateInfo allocation_info = vk::MemoryAllocateInfo()
        .setAllocationSize(memory_requirements.size)
        .setMemoryTypeIndex(
            context->find_memory_type(
                memory_requirements.memoryTypeBits,
                vk::MemoryPropertyFlagBits::eDeviceLocal
            )
        );

    if (context->logical_device.allocateMemory(&allocation_info, nullptr, &this->memory) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't allocate image memory.");
    }

    context->logical_device.bindImageMemory(this->image, this->memory, 0);
}

void Texture::transition_image_layout(vk::ImageLayout old_layout, vk::ImageLayout new_layout, uint32_t layers)
{
    vk::ImageMemoryBarrier barrier = vk::ImageMemoryBarrier()
        .setOldLayout(old_layout)
        .setNewLayout(new_layout)
        .setImage(this->image)
        .setSubresourceRange(
            vk::ImageSubresourceRange()
                .setAspectMask(vk::ImageAspectFlagBits::eColor)
                .setBaseMipLevel(0)
                .setLevelCount(1)
                .setBaseArrayLayer(0)
                .setLayerCount(layers)
        );

    if (
        old_layout == vk::ImageLayout::ePreinitialized &&
        new_layout == vk::ImageLayout::eTransferDstOptimal
    ) {
        barrier
            .setSrcAccessMask(vk::AccessFlagBits::eHostWrite)
            .setDstAccessMask(vk::AccessFlagBits::eTransferWrite);
    } else if (
        old_layout == vk::ImageLayout::eTransferDstOptimal &&
        new_layout == vk::ImageLayout::eShaderReadOnlyOptimal
    ) {
        barrier
            .setSrcAccessMask(vk::AccessFlagBits::eTransferWrite)
            .setDstAccessMask(vk::AccessFlagBits::eShaderRead);
    } else {
        throw std::runtime_error("Unsupported image layout transition requested.");
    }

    this->context.lock()->run_one_time_commands([&barrier](vk::CommandBuffer command_buffer){
        command_buffer.pipelineBarrier(
            vk::PipelineStageFlagBits::eTopOfPipe,
            vk::PipelineStageFlagBits::eTopOfPipe,
            vk::DependencyFlags(),
            0, nullptr,
            0, nullptr,
            1, &barrier
        );
    });
}

void Texture::copy_buffer_to_image(VK::Buffer &staging_buffer, std::vector<LayerData> layers, uint32_t width, uint32_t height)
{
    this->transition_image_layout(
        vk::ImageLayout::ePreinitialized,
        vk::ImageLayout::eTransferDstOptimal,
        layers.size()
    );

    std::vector<vk::BufferImageCopy> copy_regions;
    size_t offset = 0;
    uint32_t index = 0;

    for(auto const& layer : layers) {
        vk::BufferImageCopy copy_region = vk::BufferImageCopy()
            .setBufferOffset(offset)
            .setBufferRowLength(0)
            .setBufferImageHeight(0)
            .setImageSubresource(
                vk::ImageSubresourceLayers()
                    .setAspectMask(vk::ImageAspectFlagBits::eColor)
                    .setMipLevel(0)
                    .setBaseArrayLayer(index++)
                    .setLayerCount(1)
            )
            .setImageOffset({0,0,0})
            .setImageExtent({
                static_cast<uint32_t>(layer.width),
                static_cast<uint32_t>(layer.height),
                1
            });

        copy_regions.push_back(copy_region);

        offset += layer.size;
    }

    this->context.lock()->run_one_time_commands([&](vk::CommandBuffer command_buffer){
        command_buffer.copyBufferToImage(
            staging_buffer,
            image,
            vk::ImageLayout::eTransferDstOptimal,
            copy_regions.size(),
            copy_regions.data()
        );
    });

    this->transition_image_layout(
        vk::ImageLayout::eTransferDstOptimal,
        vk::ImageLayout::eShaderReadOnlyOptimal,
        layers.size()
    );
}

void Texture::create_image_view(uint32_t layers)
{
    vk::ImageViewCreateInfo view_info = vk::ImageViewCreateInfo()
        .setImage(this->image)
        .setViewType(vk::ImageViewType::e2DArray)
        .setFormat(vk::Format::eR8G8B8A8Unorm)
        .setSubresourceRange(
            vk::ImageSubresourceRange()
                .setAspectMask(vk::ImageAspectFlagBits::eColor)
                .setBaseMipLevel(0)
                .setLevelCount(1)
                .setBaseArrayLayer(0)
                .setLayerCount(layers)
        );

    if (this->context.lock()->logical_device.createImageView(&view_info, nullptr, &this->image_view) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create texture image view.");
    }
}

void Texture::create_sampler()
{
    vk::SamplerCreateInfo create_info = vk::SamplerCreateInfo()
        .setMagFilter(vk::Filter::eLinear)
        .setMinFilter(vk::Filter::eLinear)
        .setAddressModeU(vk::SamplerAddressMode::eRepeat)
        .setAddressModeV(vk::SamplerAddressMode::eRepeat)
        .setAddressModeW(vk::SamplerAddressMode::eRepeat)
        .setAnisotropyEnable(VK_TRUE)
        .setMaxAnisotropy(16)
        .setBorderColor(vk::BorderColor::eIntOpaqueBlack)
        .setUnnormalizedCoordinates(VK_FALSE)
        .setCompareEnable(VK_FALSE)
        .setCompareOp(vk::CompareOp::eAlways)
        .setMipmapMode(vk::SamplerMipmapMode::eLinear)
        .setMipLodBias(0.0f)
        .setMinLod(0.0f)
        .setMaxLod(0.0f);

    if (this->context.lock()->logical_device.createSampler(&create_info, nullptr, &this->sampler) != vk::Result::eSuccess) {
        throw std::runtime_error("Couldn't create texture sampler.");
    }
}