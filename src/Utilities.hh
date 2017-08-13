#pragma once

#include <string>

namespace Animate
{
    class Utilities
    {
        public:
            static const uint8_t * get_resource_as_bytes(std::string key, size_t *size = nullptr);
            static uint64_t get_micro_time();
    };
}
