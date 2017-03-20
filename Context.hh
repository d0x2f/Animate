#pragma once

#include <memory>

#include "VK/Textures.hh"

namespace Animate
{
    class Context
    {
        public:
            void set_textures(VK::Textures *textures);

            VK::Textures *get_textures();

        private:
            std::shared_ptr<VK::Textures> textures;
    };
}
