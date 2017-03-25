#pragma once

#include <string>
#include <map>
#include <memory>

#include "Texture.hh"

namespace Animate
{
    class Context;

    namespace VK
    {
        class Textures
        {
            public:
                Textures(std::shared_ptr<Context> context);

                Texture *get_texture(std::string resource_id);

            protected:
                std::weak_ptr<Context> context;

                std::map< std::string, std::shared_ptr<Texture> > texture_cache;
        };
    }
}
