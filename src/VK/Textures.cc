#include "Textures.hh"
#include "../AppContext.hh"

using namespace Animate::VK;

Textures::Textures(std::weak_ptr<Context> context, std::vector<std::string> resources) : context(context)
{
    this->array_texture.reset(new Texture(this->context, resources));

    uint32_t i=0;
    for(auto const& resource : resources) {
        this->texture_layers.insert(std::pair(resource, i++));
    }
}

std::weak_ptr<Texture> Textures::get_texture()
{
    return this->array_texture;
}

uint32_t Textures::get_layer(std::string resource_id)
{
    std::map< std::string, uint32_t >::iterator it = this->texture_layers.find(resource_id);

    if (it != this->texture_layers.end()) {
        return it->second;
    }

    return 0;
}

vk::ImageView Textures::get_image_view()
{
    return this->array_texture->get_image_view();
}

vk::Sampler Textures::get_sampler()
{
    return this->array_texture->get_sampler();
}