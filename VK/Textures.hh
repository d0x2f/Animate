#pragma once

#include <string>
#include <map>
#include <memory>

#include "Texture.hh"

namespace Animate
{
    class AppContext;

    namespace VK
    {
        class Textures
        {
            public:
                Textures(std::weak_ptr<AppContext> context);

                std::weak_ptr<Texture> get_texture(std::string resource_id);

            protected:
                std::weak_ptr<AppContext> context;

                std::map< std::string, std::shared_ptr<Texture> > texture_cache;
        };
    }
}
