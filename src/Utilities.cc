#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sys/time.h>

#include "Utilities.hh"
#include "Resources.hh"

using namespace Animate;

const uint8_t * Utilities::get_resource_as_bytes(std::string key, size_t *size)
{
    auto search = Resources::resources.find(key);

    if (search == Resources::resources.end()) {
        throw std::runtime_error("Resource not found: " + key);
    }

    if (size != nullptr) {
        *size = search->second.size();
    }
    return search->second.data();
}

uint64_t Utilities::get_micro_time()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return ((uint64_t)time.tv_sec * 1000000) + time.tv_usec;
}
