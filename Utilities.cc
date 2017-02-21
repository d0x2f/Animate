#include "Utilities.hh"
#include "../resources.h"

using namespace Animate;

gconstpointer Utilities::get_resource_as_bytes(std::string key)
{
    Glib::RefPtr< const Glib::Bytes > bytes = Gio::Resource::lookup_data_global(key);
    gsize size = bytes->get_size();
    return bytes->get_data(size);
}
