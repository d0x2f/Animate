#include "Textures.hh"
#include "../AppContext.hh"

using namespace Animate::VK;

Textures::Textures(std::weak_ptr<AppContext> context) : context(context)
{
    this->context.lock()->set_textures(this);
}

std::weak_ptr<Texture> Textures::get_texture(std::string resource_id)
{
    std::map< std::string, std::shared_ptr<Texture> >::iterator it;
    it = this->texture_cache.find(resource_id);

    if (it == this->texture_cache.end()) {
        std::shared_ptr<Texture> texture = std::shared_ptr<Texture>(
            new Texture(
                this->context.lock()->get_graphics_context(),
                resource_id
            )
        );
        this->texture_cache.insert(
            std::pair< std::string, std::shared_ptr<Texture> >(resource_id, texture)
        );
        return texture;
    }

    return it->second;
}
