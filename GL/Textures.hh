#pragma once

#include <string>
#include <map>
#include <memory>

#include "Texture.hh"

namespace Animate
{
    class Context;

    namespace GL
    {
        class Textures
        {
            public:
                Textures(Context *context);

                Texture *get_texture(std::string resource_id);

            protected:
                Context *context;

                std::map< std::string, std::shared_ptr<Texture> > texture_cache;
        };
    }
}
