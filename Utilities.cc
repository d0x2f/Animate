#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include "Utilities.hh"
#include "../resources.h"
#include <sys/time.h>

using namespace Animate;

gconstpointer Utilities::get_resource_as_bytes(std::string key, size_t *size)
{
    Glib::RefPtr< const Glib::Bytes > bytes = Gio::Resource::lookup_data_global(key);
    size_t resource_size = bytes->get_size();
    if (size != nullptr) {
        *size = resource_size;
    }
    return bytes->get_data(resource_size);
}

uint64_t Utilities::get_micro_time()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return ((uint64_t)time.tv_sec * 1000000) + time.tv_usec;
}
