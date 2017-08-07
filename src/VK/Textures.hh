#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>

#include "Texture.hh"

namespace Animate::VK
{
    class Textures
    {
        public:
            Textures(std::weak_ptr<Context> context, std::vector<std::string> resources);

            std::weak_ptr<Texture> get_texture();
            uint32_t get_layer(std::string resource_id);

            vk::ImageView get_image_view();
            vk::Sampler get_sampler();

        protected:
            std::weak_ptr<Context> context;

            std::shared_ptr<Texture> array_texture;

            std::map< std::string, uint32_t> texture_layers;
    };
}
