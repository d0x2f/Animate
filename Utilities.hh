#pragma once

#include <string>
#include <giomm/resource.h>

namespace Animate
{
    class Utilities
    {
        public:
            static gconstpointer get_resource_as_bytes(std::string key, size_t *size = nullptr);
            static uint64_t get_micro_time();
    };
}
