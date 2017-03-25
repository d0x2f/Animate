#include "Textures.hh"
#include "../Context.hh"

using namespace Animate::VK;

Textures::Textures(std::shared_ptr<Context> context) : context(context)
{
    this->context.lock()->set_textures(this);
}

Texture *Textures::get_texture(std::string resource_id)
{
    std::map< std::string, std::shared_ptr<Texture> >::iterator it;
    it = this->texture_cache.find(resource_id);

    if (it == this->texture_cache.end()) {
        Texture *texture = new Texture();
        texture->load_from_resource(resource_id);
        this->texture_cache.insert(
            std::pair< std::string, std::shared_ptr<Texture> >(resource_id, texture)
        );
        return texture;
    }

    return it->second.get();
}
