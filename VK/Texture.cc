#include <gtkmm-3.0/gtkmm.h>
#include <iostream>

#include "Texture.hh"
#include "Context.hh"
#include "Buffer.hh"
#include "../Utilities.hh"

#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"

using namespace Animate::VK;

Texture::Texture(std::weak_ptr<Context> context, std::vector<std::string> resources) : context(context)
{
    this->logical_device = context.lock()->logical_device;
    
    Glib::RefPtr<Gdk::Pixbuf> pixbuf;

    vk::DeviceSize size;
    int width;
    int height;
    int channels;
    size_t byte_length;

    uint8_t const *bytes = reinterpret_cast<const uint8_t*>(Utilities::get_resource_as_bytes(resources.front(), &byte_length));

    stbi_uc* pixels = stbi_load_from_memory(
        bytes,
        static_cast<int>(byte_length),
        &width,
        &height,
        &channels,
        STBI_rgb_alpha
    );

    if (!pixels) {
        throw std::runtime_error("Couldn't load texture resource: " + resources.front());
    }
    
    vk::DeviceSize destination_size = 4 * width * height;

    VK::Buffer staging_buffer(
        this->context.lock(),
        destination_size,
        vk::BufferUsageFlagBits::eTransferSrc,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
    );

    void *data = staging_buffer.map();
    memcpy(data, pixels, static_cast<size_t>(destination_size));
    staging_buffer.unmap();
    
    this->create_image(width, height);
    this->copy_buffer_to_image(staging_buffer, width, height);
    this->create_image_view();
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

vk::ImageView Texture::get_image_view()
{
    return this->image_view;
}

vk::Sampler Texture::get_sampler()
{
    return this->sampler;
}

void Texture::create_image(uint32_t width, uint32_t height)
{
    vk::ImageCreateInfo create_info = vk::ImageCreateInfo()
        .setImageType(vk::ImageType::e2D)
        .setExtent({width, height, 1})
        .setMipLevels(1)
        .setArrayLayers(1)
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

void Texture::transition_image_layout(vk::ImageLayout old_layout, vk::ImageLayout new_layout)
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
                .setLayerCount(1)
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

void Texture::copy_buffer_to_image(VK::Buffer &staging_buffer, uint32_t width, uint32_t height)
{
    this->transition_image_layout(
        vk::ImageLayout::ePreinitialized,
        vk::ImageLayout::eTransferDstOptimal
    );

    vk::BufferImageCopy copy_region = vk::BufferImageCopy()
        .setBufferOffset(0)
        .setBufferRowLength(0)
        .setBufferImageHeight(0)
        .setImageSubresource(
            vk::ImageSubresourceLayers()
                .setAspectMask(vk::ImageAspectFlagBits::eColor)
                .setMipLevel(0)
                .setBaseArrayLayer(0)
                .setLayerCount(1)
        )
        .setImageOffset({0,0,0})
        .setImageExtent({width, height, 1});

    this->context.lock()->run_one_time_commands([&](vk::CommandBuffer command_buffer){
        command_buffer.copyBufferToImage(
            staging_buffer,
            image,
            vk::ImageLayout::eTransferDstOptimal,
            1,
            &copy_region
        );
    });

    this->transition_image_layout(
        vk::ImageLayout::eTransferDstOptimal,
        vk::ImageLayout::eShaderReadOnlyOptimal
    );
}

void Texture::create_image_view()
{
    vk::ImageViewCreateInfo view_info = vk::ImageViewCreateInfo()
        .setImage(this->image)
        .setViewType(vk::ImageViewType::e2D)
        .setFormat(vk::Format::eR8G8B8A8Unorm)
        .setSubresourceRange(
            vk::ImageSubresourceRange()
                .setAspectMask(vk::ImageAspectFlagBits::eColor)
                .setBaseMipLevel(0)
                .setLevelCount(1)
                .setBaseArrayLayer(0)
                .setLayerCount(1)
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