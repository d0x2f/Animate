#include "Textures.hh"
#include "../Context.hh"

using namespace Animate::GL;

Textures::Textures(Context *context) : context(context)
{
    context->set_textures(this);
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