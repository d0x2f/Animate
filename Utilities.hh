#pragma once

#include <string>
#include <giomm/resource.h>

namespace Animate
{
    class Utilities
    {
        public:
            static gconstpointer get_resource_as_bytes(std::string key);
    };
}
