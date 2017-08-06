#include "Textures.hh"
#include "../AppContext.hh"

using namespace Animate::VK;

Textures::Textures(std::weak_ptr<Context> context, std::vector<std::string> resources) : context(context)
{
    this->array_texture = std::shared_ptr<Texture>(new Texture(this->context, resources));
}

std::weak_ptr<Texture> Textures::get_texture()
{
    return this->array_texture;
}

uint32_t Textures::get_layer(std::string resource_id)
{
    return 0;
    
    std::map< std::string, uint32_t >::iterator it = this->texture_layers.find(resource_id);

    if (it != this->texture_layers.end()) {
        return it->second;
    }
}

vk::ImageView Textures::get_image_view()
{
    return this->array_texture->get_image_view();
}

vk::Sampler Textures::get_sampler()
{
    return this->array_texture->get_sampler();
}