#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Utilities.hh"
#include "../resources.h"
#include <sys/time.h>

using namespace Animate;

gconstpointer Utilities::get_resource_as_bytes(std::string key)
{
    Glib::RefPtr< const Glib::Bytes > bytes = Gio::Resource::lookup_data_global(key);
    gsize size = bytes->get_size();
    return bytes->get_data(size);
}

GLuint64 Utilities::get_micro_time()
{
    struct timeval time;
    gettimeofday(&time, NULL);
    return ((GLuint64)time.tv_sec * 1000000) + time.tv_usec;
}
